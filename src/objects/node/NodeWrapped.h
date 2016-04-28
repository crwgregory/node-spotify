#ifndef _NODE_WRAPPED_H
#define _NODE_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>
#include <nan.h>

/**
 * A class used as a base class for wrapping objects to node objects.
 **/
template <class T>
class NodeWrapped : public Nan::ObjectWrap {
public:
  ~NodeWrapped() {}
  
  virtual v8::Handle<v8::Object> createInstance() {
    v8::Local<v8::Object> object = Nan::New(constructor)->NewInstance();
    this->Wrap(object);
    return object;
  }

  static Nan::Persistent<v8::Function> getConstructor() {
    return constructor; 
  }
protected:
  static Nan::Persistent<v8::Function> constructor;

  /**
   * Basic init method for a wrapped node object.
   */
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::FunctionTemplate> constructorTemplate = Nan::New<v8::FunctionTemplate>();
    constructorTemplate->SetClassName(Nan::New<v8::String>(className).ToLocalChecked());
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    return scope.Escape(constructorTemplate);
  }
};

//The constructor template must be static per template instance not for all NodeWrapped subclasses.
template <class T> Nan::Persistent<v8::Function> NodeWrapped<T>::constructor;
#endif
