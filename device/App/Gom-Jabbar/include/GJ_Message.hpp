// ________  ________  _____ ______                             
// |\   ____\|\   __  \|\   _ \  _   \                           
// \ \  \___|\ \  \|\  \ \  \\\__\ \  \                          
//  \ \  \  __\ \  \\\  \ \  \\|__| \  \                         
//   \ \  \|\  \ \  \\\  \ \  \    \ \  \                        
//    \ \_______\ \_______\ \__\    \ \__\                       
//     \|_______|\|_______|\|__|_____\|__|________  ________     
//    |\  \|\   __  \|\   __  \|\   __  \|\   __  \|\   __  \    
//    \ \  \ \  \|\  \ \  \|\ /\ \  \|\ /\ \  \|\  \ \  \|\  \   
//  __ \ \  \ \   __  \ \   __  \ \   __  \ \   __  \ \   _  _\  
// |\  \\_\  \ \  \ \  \ \  \|\  \ \  \|\  \ \  \ \  \ \  \\  \| 
// \ \________\ \__\ \__\ \_______\ \_______\ \__\ \__\ \__\\ _\ 
//  \|________|\|__|\|__|\|_______|\|_______|\|__|\|__|\|__|\|__|
// Message objects, stores elements of JSON input for internal use
// Message Handler, manages ingoing and outgoing data streams
#pragma once
#include "GJ_Config.hpp"
#include "GJ_Types.hpp"
#include "yyjson.h"

#include <string.h>
#include <vector>

namespace GJ
{
    class Message
    {
        private:
            MessageType type_;
            yyjson_doc * data_;
            const char * serialized_;
        public:

            Message() = default;
            Message( const char * msgString ) : serialized_(msgString){};
            ~Message() = delete;

            inline MessageType type()
            {
                return type_;
            }
            /*
            * Writes yyjson doc to serialized string
            */
            const char * serialize()
            {
                return yyjson_write(data_, 0, NULL);
            }
            
            /*
            * Reads msgString into yyjson doc
            */
            void deserialize()
            {
                yyjson_read_err err_;
                data_ = yyjson_read_opts((char *)serialized_, strlen(serialized_), 0, NULL, &err_ );
                if(!data_)
                {
                    char buffer[100];
                    snprintf(buffer, sizeof(buffer), "JSON READ ERROR: %s:%d code: %d\n", err_.msg, err_.pos, err_.code);
                    printf("%s\n", buffer);
                }
            }
            /*
            * Reads the document root and determines the message type
            */
            void set_type()
            {
                if(!data_)
                {
                    return;
                }
                yyjson_val * root = yyjson_doc_get_root(data_);
                yyjson_val * ptr;
                ptr = yyjson_obj_get(root, "test_manifest" );
                if(ptr != nullptr)
                {
                   type_ =  MANIFEST;
                   return;
                }
                ptr = yyjson_obj_get(root, "session_start" );
                if(ptr != nullptr)
                {
                    printf("SESSION START\n");
                   type_ = SESSION_START;
                   return;
                }
                ptr = yyjson_obj_get(root, "test_request" );
                if(ptr != nullptr)
                {
                   type_ = TEST_REQUEST;
                   return;
                }
            
                type_ = UNKNOWN;
                return;
            }
    };
    /*
    * Class responsible for reading messages from userspace to internal message handler
    * Our incoming data pipe is as follows: 
    * 
    *   User defined string acceptance protocol -> Message parsing Queue -> Component(whichever the recipient is)
    * 
    * Out going data pipeline:
    *   
    *   Component -> jsonwriter -> message handler -> user defined message sender (hardware specific)
    * 
    * The messages can also infer message type based on incoming JSON schema (see desktop/message_examples)
    * 
    * Message Handler publishes data from the incoming stream to components, and accepts data for the outgoing stream
    * from components
    * 
    * It is responsible for managing these two data streams, and ensuring that data is read safely
    * 
    */
   class MessageHandler
   {
       private: 
           std::vector< Message*> * incoming_;
           std::vector< Message*> * outgoing_;
           Config * config_;
       
       public:
            MessageHandler(Config * config) 
            : config_   (config)
            , incoming_ (new std::vector<Message*>())
            , outgoing_ (new std::vector<Message*>())
            {}
            
            void queue_outgoing(Message msg)
            {
                outgoing_->push_back(&msg);
            }
           /*
           * Sends an outgoing message, just one, then deletes the message from outgoing queue
           */
            void send_outgoing()
            {
                if(outgoing_->empty())
                {
                    return;
                }
                config_->send(outgoing_->at(0)->serialize());
                outgoing_->erase(outgoing_->begin());
            }

            /*
           * Sends all outgoing messages, then dump the queue
           */
            void send_all_outgoing()
            {
                while(!outgoing_->empty())
                {
                    config_->send(outgoing_->at(0)->serialize());
                    outgoing_->erase(outgoing_->begin());
                }
            }

           /*
           * Register this to an interrupt for when 
           * whatever message buffer you have is full
           * Will receive a message
           */
           void queue_incoming()
           {
                if(!config_->input_ready())
                {
                    return;
                }
                Message * msg = new Message(config_->receive());
                incoming_->push_back(msg);
                incoming_->back()->deserialize();
                incoming_->back()->set_type();
        
            }
   };
}