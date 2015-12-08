#include "NodeTrack.h"
#include "NodeArtist.h"
#include "NodeAlbum.h"

NodeTrack::NodeTrack(std::shared_ptr<Track> _track) : track(_track) {

};

NodeTrack::~NodeTrack() {

}

NAN_GETTER(NodeTrack::getName) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<String>(nodeTrack->track->name().c_str()));
}

NAN_GETTER(NodeTrack::getLink) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<String>(nodeTrack->track->link().c_str()));
}

NAN_GETTER(NodeTrack::getDuration) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeTrack->track->duration()/1000));
}

NAN_GETTER(NodeTrack::getAvailability) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeTrack->track->getAvailability()));
}

NAN_GETTER(NodeTrack::getPopularity) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeTrack->track->popularity()));
}

NAN_GETTER(NodeTrack::getArtists) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  Local<Array> jsArtists = Nan::New<Array>(nodeTrack->track->artists().size());
  for(int i = 0; i < (int)nodeTrack->track->artists().size(); i++) {
    if(nodeTrack->track->artists()[i]) {
      NodeArtist* nodeArtist = new NodeArtist(std::move(nodeTrack->track->artists()[i]));
      jsArtists->Set(Nan::New<Number>(i), nodeArtist->createInstance());
    } else {
      jsArtists->Set(Nan::New<Number>(i), NanUndefined());
    }
  }
  NanReturnValue(jsArtists);
}

NAN_GETTER(NodeTrack::getAlbum) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  if(nodeTrack->track->album()) {
    NodeAlbum* nodeAlbum = new NodeAlbum(nodeTrack->track->album());
    NanReturnValue(nodeAlbum->createInstance());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(NodeTrack::getStarred) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<Boolean>(nodeTrack->track->starred()));
}

NAN_SETTER(NodeTrack::setStarred) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  nodeTrack->track->setStarred(value->ToBoolean()->Value());
}

NAN_GETTER(NodeTrack::isLoaded) {
  NanScope();
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args.This());
  NanReturnValue(Nan::New<Boolean>(nodeTrack->track->isLoaded()));
}

Handle<FunctionTemplate> NodeTrack::init() {
  NanEscapableScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Track");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name"), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("duration"), getDuration);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("artists"), getArtists);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("album"), getAlbum);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("starred"), getStarred, setStarred);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("popularity"), getPopularity);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded"), isLoaded);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("availability"), getAvailability);
  NanAssignPersistent(NodeTrack::constructorTemplate, constructorTemplate);
  return NanEscapeScope(constructorTemplate);
}
