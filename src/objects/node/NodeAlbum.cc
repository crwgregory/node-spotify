#include "NodeAlbum.h"
#include "NodeTrack.h"
#include "NodeArtist.h"
#include "../spotify/Track.h"

NodeAlbum::NodeAlbum(std::unique_ptr<Album> _album) : album(std::move(_album)) {
  album->nodeObject = this;
};

NodeAlbum::~NodeAlbum() {
  if(album->nodeObject == this) {
    album->nodeObject = nullptr;
  }
}

NAN_GETTER(NodeAlbum::getName) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->name().c_str()));
}

NAN_GETTER(NodeAlbum::getLink) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->link().c_str()));
}

NAN_METHOD(NodeAlbum::getCoverBase64) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->coverBase64().c_str()));
}

NAN_METHOD(NodeAlbum::browse) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  if(nodeAlbum->album->albumBrowse == nullptr) {
    nodeAlbum->makePersistent();
    nodeAlbum->browseCompleteCallback = std::unique_ptr<Nan::Callback>(new Nan::Callback(args[0].As<Function>()));

    //Mutate the V8 object.
    Handle<Object> nodeAlbumV8 = NanObjectWrapHandle(nodeAlbum);
    nodeAlbumV8->SetAccessor(Nan::New<String>("tracks").ToLocalChecked(), getTracks);
    nodeAlbumV8->SetAccessor(Nan::New<String>("review").ToLocalChecked(), getReview);
    nodeAlbumV8->SetAccessor(Nan::New<String>("copyrights").ToLocalChecked(), getCopyrights);
    nodeAlbumV8->SetAccessor(Nan::New<String>("artist").ToLocalChecked(), getArtist);

    nodeAlbum->album->browse();
  } else {
    nodeAlbum->callBrowseComplete();
  }
  NanReturnUndefined();
}

NAN_GETTER(NodeAlbum::getTracks) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeAlbum->album->tracks();
  Handle<Array> nodeTracks = Nan::New<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTracks);
}

NAN_GETTER(NodeAlbum::getReview) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->review().c_str()));
}

NAN_GETTER(NodeAlbum::getCopyrights) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  std::vector<std::string> copyrights = nodeAlbum->album->copyrights();
  Handle<Array> nodeCopyrights = Nan::New<Array>(copyrights.size());
  for(int i = 0; i < (int)copyrights.size(); i++) {
    nodeCopyrights->Set(Nan::New<Number>(i), Nan::New<String>(copyrights[i].c_str()));
  }
  NanReturnValue(nodeCopyrights);
}

NAN_GETTER(NodeAlbum::getArtist) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NodeArtist* nodeArtist = new NodeArtist(nodeAlbum->album->artist());
  NanReturnValue(nodeArtist->createInstance());
}

NAN_GETTER(NodeAlbum::isLoaded) {
  NanScope();
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  NanReturnValue(Nan::New<Boolean>(nodeAlbum->album->isLoaded()));
}

void NodeAlbum::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Album");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name").ToLocalChecked(), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCoverBase64", getCoverBase64);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  NanAssignPersistent(NodeAlbum::constructorTemplate, constructorTemplate);
}
