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
// Json reader and message queue

#pragma once

/*std includes*/
#include <string>
#include <deque>
#include <memory>

/*external includes*/
#include "yyjson.h"

namespace GJ
{
    /*
    * Queue that stores JSON objects waiting to be read by tasks or whatever
    * Is static because we only want one
    */
    class JsonQueue
    {
        private:
            std::deque<yyjson_doc> docs_;
            //Constructor is private, use get() instead.
            JsonQueue(){};

        public:

            /*
            * gets the instance of the queue
            * if no instance exists, create one
            * we do NOT want the accidental creation
            * of more than one
            */
            static JsonQueue * getInstance()
            {
                static JsonQueue instance;
                return &instance;
            }
            /*
            * Add() - adds a json document to the FILO queue
            */
            void add( yyjson_doc * doc )
            {
                docs_.push_back(*doc);
            }
            /*
            * peek() - returns a pointer to the object in queue and does not delete the object
            * THIS WILL SEG FAULT IF ANOTHER THREAD CALLS read() DIRECTLY AFTER
            * Ideally, you would peek() a message and see if it belongs to you, if so
            * you can read() it, which will destroy its memory in the queue
            */
            inline yyjson_doc * peek()
            {
                return &docs_.front();
            }
            /*
            * read() - removes an item from the queue and returns a copy of the object for local use
            * the memory is then managed by the caller of read()
            */
            inline yyjson_doc * read()
            {
                yyjson_doc * doc;
                memcpy(doc, &docs_.front(), sizeof(docs_.front()));
                docs_.pop_front();
                return doc;
            }
    };
    /*
    * Class that reads in raw string into JSON
    * Devs are responsible for reading input into a string,
    * and passing it to this class, we take it from there
    */
    class JsonReader
    {
        private:
            std::string raw_;
            yyjson_read_err err_;

        public:
            JsonReader(){};
            /*
            * Push a character onto the back of the internal string
            * useful for methods that run in IRQ one char at a time
            */
            inline void push(char character)
            {
                raw_.push_back(character);
            }

            /*
            * Sets the entirety of the strings contents to this string
            * works for both c_str and string types
            */
            inline void set( char * str)
            {
                std::string _string = std::string(str);
                raw_.swap(_string);
            }
            inline void set( std::string * str)
            {
                raw_.swap(*str);
            }
            /*
            * Deserializes the JSON into the internal yyJSON object
            * and puts it into internal FILO queue
            * Includes error handling
            */
            void deserialize()
            {
                yyjson_doc * jsonObj_ = yyjson_read_opts((char *)raw_.c_str(), raw_.length(), 0, NULL, &err_ );
                
                if(!jsonObj_)
                {//TODO: send error to generic messenger interface instead
                    printf("JSON read error: %s, code: %u at byte position: %lu\n", err_.msg, err_.code, err_.pos);
                }
                else
                {
                    JsonQueue::getInstance()->add(jsonObj_);
                }
                //Clear out the string buffer and ready for the next object
                raw_.clear(); 
            }
            /*
            * Once we decide on a schema to pass messages back and forth, we can add more here
            */
    };


}