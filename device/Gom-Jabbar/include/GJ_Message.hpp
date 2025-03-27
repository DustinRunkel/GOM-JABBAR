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

#include "GJ_Types.hpp"
#include "GJ_Json.hpp"

#include <deque> 
#include <atomic>

namespace GJ
{

    class Message
    {
        private:
            MessageType type_;


            Message(){};

        public:
            yyjson_doc  data_;

            Message( MessageType type, yyjson_doc data )
            {
                type_ = type;
                data_ = data;
            }

            Message( yyjson_doc data )
            {
                data_ = data;
            }

            inline MessageType type()
            {
                return type_;
            }

            const char * serialize()
            {
                return yyjson_write(&data_, 0, NULL);
            }

    };
    /*
    * Class responsible for reading messages from the JSON queue into memory.
    * Our incoming data pipe is as follows: 
    * 
    *   User defined string acceptance protocol -> Json parsing Queue -> Message parsing Queue -> Component(whichever the recipient is)
    * 
    * Out going data pipeline:
    *   
    *   Component -> jsonwriter -> message handler -> user defined message sender (hardware specific)
    * 
    * This is done to decouple the json parsing and message reading
    * The messages can also infer message type based on incoming JSON schema
    * 
    * Message Handler publishes data from the incoming stream to components, and accepts data for the outgoing stream
    * from components
    * 
    * It is responsible for managing these two data streams atomically. 
    * 
    */
   class MessageHandler
   {
       private: 
           std::deque<std::atomic<Message>> incoming_;
           std::deque<std::atomic<Message>> outgoing_;
           JsonReader jsonReader_;
       
       public:
           /*
           * Overloadable message interface
           * User is required to provide a hardware specific
           * Protocol in which serialized output is written
           */
           virtual void send(const char * msg) = 0;
           /*
           * Overloadable message receive function
           * allows user to create board-specific implementations
           */
           virtual char * receive() = 0;
           /*
           * Sends an outgoing message, just one, then deletes the message from outgoing queue
           */
           virtual void send_outgoing()
           {
                send(outgoing_.at(0).load().serialize());
                outgoing_.pop_front();
           }
           /*
           * Register this to an interrupt for when 
           * whatever message buffer you have is full
           * Will receive a message, and trigger the
           * parser, which then puts the JSON document into the
           * FILO queue
           */
           virtual void receive_incoming()
           {
                char * serialized_msg = receive();
                if( serialized_msg == nullptr )
                {//If you get here, your virtual message handler is busted.
                    return;
                }
                jsonReader_.set(serialized_msg);
                //message is pushed into the queue
                jsonReader_.deserialize();
           }
       
       
   };
}