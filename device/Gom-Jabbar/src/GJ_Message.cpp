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

#include "GJ_Message.hpp"
#include "GJ_Config.hpp"

namespace GJ 
{

Message::Message(const char * str) : serialized_ (str)
{}

const char * Message::serialize()
{
    return yyjson_write(data_, 0, NULL);
}

void Message::deserialize()
{
    yyjson_read_err err_;
    data_ = yyjson_read_opts((char *)serialized_, sizeof(serialized_), 0, NULL, &err_ );
    
    if(!data_)
    {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "JSON READ ERROR: %s:%d code: %d\n", err_.msg, err_.pos, err_.code);
        printf("%s\n", buffer);
    }
}

void Message::set_type()
{
    if(!data_)
    {
        return;
    }

    yyjson_val * root = yyjson_doc_get_root(data_);
    yyjson_val * ptr;
    ptr = yyjson_obj_get(root, "test_manifest" );
    if(ptr)
    {
       type_ =  MANIFEST;
       return;
    }
    ptr = yyjson_obj_get(root, "start_session" );
    if(ptr)
    {
        printf("SESSION START\n");
       type_ = SESSION_START;
       return;
    }
    ptr = yyjson_obj_get(root, "test_request" );
    if(ptr)
    {
       type_ = TEST_REQUEST;
       return;
    }

    if(ptr)
    {
        type_ = UNKNOWN;
        return;
    }
}

MessageHandler::MessageHandler(Config * config)
: config_ (config)
{}

void MessageHandler::queue_outgoing(Message msg)
{
    outgoing_->push_back(&msg);
}

void MessageHandler::send_outgoing()
{
    if(outgoing_->empty())
    {
        return;
    }
    config_->send(outgoing_->at(0)->serialize());
    outgoing_->erase(outgoing_->begin());
}

void MessageHandler::send_all_outgoing()
{
    while(!outgoing_->empty())
    {
        config_->send(outgoing_->at(0)->serialize());
        outgoing_->erase(outgoing_->begin());
    }
}

void MessageHandler::queue_incoming()
{
    if(!config_->input_ready())
    {
        return;
    }
    printf("Entering Queue\n");
    Message * msg = new Message(config_->receive());
    printf("Message created\n");
    incoming_->push_back(msg);
    printf("Message Queued\n");
    //Populate msg internal json structure
    //incoming_.back()->deserialize();
    //printf("Message Deserialized\n");
    incoming_->back()->set_type();
    printf("Message type detected\n");

}

} //end namespace