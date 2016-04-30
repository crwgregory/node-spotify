#include "NodeSearch.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"
#include "NodeArtist.h"
#include "NodePlaylist.h"
#include "../../Application.h"

extern Application* application;

NodeSearch::NodeSearch(const char* _searchQuery) : searchQuery(_searchQuery), trackOffset(0), albumOffset(0), artistOffset(0), playlistOffset(0),
  trackLimit(10), albumLimit(10), artistLimit(10), playlistLimit(10) {

}

NodeSearch::NodeSearch(const char* _searchQuery, int offset) : searchQuery(_searchQuery), trackOffset(offset), albumOffset(offset), artistOffset(offset), playlistOffset(offset),
  trackLimit(10), albumLimit(10), artistLimit(10), playlistLimit(10) {

}

NodeSearch::NodeSearch(const char* _searchQuery, int offset, int limit) : searchQuery(_searchQuery), trackOffset(offset), albumOffset(offset), artistOffset(offset), playlistOffset(offset),
  trackLimit(limit), albumLimit(limit), artistLimit(limit), playlistLimit(limit) {

}

NAN_METHOD(NodeSearch::execute) {
  NanScope();
  if(args.Length() < 1) {//TODO: how to check if it is a function? ->IsFunction() does not work, it does not recoginze functions.
    return Nan::ThrowError("execute needs a callback function as its argument.");
  }
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->makePersistent();
  nodeSearch->browseCompleteCallback = std::unique_ptr<Nan::Callback>(new Nan::Callback(args[0].As<Function>()));
  nodeSearch->search = std::unique_ptr<Search>(new Search());
  nodeSearch->search->nodeObject = nodeSearch;
  nodeSearch->search->execute(nodeSearch->searchQuery, nodeSearch->trackOffset, nodeSearch->trackLimit,
    nodeSearch->albumOffset, nodeSearch->albumLimit,
    nodeSearch->artistOffset, nodeSearch->artistLimit,
    nodeSearch->playlistLimit, nodeSearch->playlistLimit);
  nodeSearch->setupAdditionalMethods();
  NanReturnUndefined();
}

/**
 * Adds adiitional properties to the V8 object.
 * These will call libspotify functions and should first be available when the search has been executed.
 **/
void NodeSearch::setupAdditionalMethods() {
  Handle<Object> nodeObject = NanObjectWrapHandle(this);
  nodeObject->SetAccessor(Nan::New<String>("didYouMean").ToLocalChecked(), didYouMean);
  nodeObject->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  nodeObject->Set(Nan::New<String>("getTrack").ToLocalChecked(), Nan::New<FunctionTemplate>(getTrack)->GetFunction());
  nodeObject->Set(Nan::New<String>("getAlbum").ToLocalChecked(), Nan::New<FunctionTemplate>(getAlbum)->GetFunction());
  nodeObject->Set(Nan::New<String>("getArtist").ToLocalChecked(), Nan::New<FunctionTemplate>(getArtist)->GetFunction());
  nodeObject->Set(Nan::New<String>("getPlaylist").ToLocalChecked(), Nan::New<FunctionTemplate>(getPlaylist)->GetFunction());
  nodeObject->SetAccessor(Nan::New<String>("totalTracks").ToLocalChecked(), getTotalTracks);
  nodeObject->SetAccessor(Nan::New<String>("numTracks").ToLocalChecked(), getNumTracks);
  nodeObject->SetAccessor(Nan::New<String>("totalAlbums").ToLocalChecked(), getTotalAlbums);
  nodeObject->SetAccessor(Nan::New<String>("numAlbums").ToLocalChecked(), getNumAlbums);
  nodeObject->SetAccessor(Nan::New<String>("totalArtists").ToLocalChecked(), getTotalArtists);
  nodeObject->SetAccessor(Nan::New<String>("numArtists").ToLocalChecked(), getNumArtists);
  nodeObject->SetAccessor(Nan::New<String>("totalPlaylists").ToLocalChecked(), getTotalPlaylists);
  nodeObject->SetAccessor(Nan::New<String>("numPlaylists").ToLocalChecked(), getNumPlaylists);
}

NAN_GETTER(NodeSearch::getTrackOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->trackOffset));
}

NAN_SETTER(NodeSearch::setTrackOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->trackOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getAlbumOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->albumOffset));
}

NAN_SETTER(NodeSearch::setAlbumOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->albumOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getArtistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->artistOffset));
}

NAN_SETTER(NodeSearch::setArtistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->artistOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getPlaylistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->playlistOffset));
}

NAN_SETTER(NodeSearch::setPlaylistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->playlistOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getTrackLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->trackLimit));
}

NAN_SETTER(NodeSearch::setTrackLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->trackLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getAlbumLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->albumLimit));
}

NAN_SETTER(NodeSearch::setAlbumLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->albumLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getArtistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->artistLimit));
}

NAN_SETTER(NodeSearch::setArtistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->artistLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getPlaylistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->playlistLimit));
}

NAN_SETTER(NodeSearch::setPlaylistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->playlistLimit = value->ToInteger()->Value();
}

NAN_METHOD(NodeSearch::New) {
  NanScope();
  NodeSearch* search;
  NanUtf8String searchQuery(args[0]->ToString());
  if(args.Length() == 1) {
    search = new NodeSearch(*searchQuery);
  } else if(args.Length() == 2) {
    int offset = args[1]->ToInteger()->Value();
    search = new NodeSearch(*searchQuery, offset);
  } else if(args.Length() == 3) {
    int offset = args[1]->ToInteger()->Value();
    int limit = args[2]->ToInteger()->Value();
    search = new NodeSearch(*searchQuery, offset, limit);
  } else {
    return Nan::ThrowError("Please provide an object to the node-spotify initializer function");
  }
  search->Wrap(args.This());
  NanReturnThis();
}

NAN_GETTER(NodeSearch::didYouMean) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<String>(nodeSearch->search->didYouMeanText().c_str()));
}

NAN_GETTER(NodeSearch::getLink) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<String>(nodeSearch->search->link().c_str()));
}

NAN_METHOD(NodeSearch::getTrack) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return Nan::ThrowError("getTrack needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numTracks() || position < 0) {
    return Nan::ThrowError("Track index out of bounds");
  }

  NodeTrack* nodeTrack = new NodeTrack(nodeSearch->search->getTrack(position));
  NanReturnValue(nodeTrack->createInstance());
}

NAN_METHOD(NodeSearch::getAlbum) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return Nan::ThrowError("getAlbum needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numAlbums() || position < 0) {
    return Nan::ThrowError("Album index out of bounds");
  }

  NodeAlbum* nodeAlbum = new NodeAlbum(nodeSearch->search->getAlbum(position));
  NanReturnValue(nodeAlbum->createInstance());
}

NAN_METHOD(NodeSearch::getArtist) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return Nan::ThrowError("getArtist needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numArtists() || position < 0) {
    return Nan::ThrowError("Artist index out of bounds");
  }

  NodeArtist* nodeArtist = new NodeArtist(nodeSearch->search->getArtist(position));
  NanReturnValue(nodeArtist->createInstance());
}

NAN_METHOD(NodeSearch::getPlaylist) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return Nan::ThrowError("getPlaylist needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numPlaylists() || position < 0) {
    return Nan::ThrowError("Playlist index out of bounds");
  }

  NodePlaylist* nodePlaylist = new NodePlaylist(nodeSearch->search->getPlaylist(position));
  NanReturnValue(nodePlaylist->createInstance());
}

NAN_GETTER(NodeSearch::getTotalTracks) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->totalTracks()));
}

NAN_GETTER(NodeSearch::getNumTracks) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->numTracks()));
}

NAN_GETTER(NodeSearch::getTotalAlbums) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->totalAlbums()));
}

NAN_GETTER(NodeSearch::getNumAlbums) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->numAlbums()));
}

NAN_GETTER(NodeSearch::getTotalArtists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->totalArtists()));
}

NAN_GETTER(NodeSearch::getNumArtists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->numArtists()));
}

NAN_GETTER(NodeSearch::getTotalPlaylists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->totalPlaylists()));
}

NAN_GETTER(NodeSearch::getNumPlaylists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(Nan::New<Integer>(nodeSearch->search->numPlaylists()));
}

void NodeSearch::init() {
  NanScope();
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>(New);
  constructorTemplate->SetClassName(Nan::New<String>("Search").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "execute", execute);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("trackOffset").ToLocalChecked(), getTrackOffset, setTrackOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("trackLimit").ToLocalChecked(), getTrackLimit, setTrackLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("albumOffset").ToLocalChecked(), getAlbumOffset, setAlbumOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("albumLimit").ToLocalChecked(), getAlbumLimit, setAlbumLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("artistOffset").ToLocalChecked(), getArtistOffset, setArtistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("artistLimit").ToLocalChecked(), getArtistLimit, setArtistLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("playlistOffset").ToLocalChecked(), getPlaylistOffset, setPlaylistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("playlistLimit").ToLocalChecked(), getPlaylistLimit, setPlaylistLimit);
  NanAssignPersistent(NodeSearch::constructorTemplate, constructorTemplate);
}
