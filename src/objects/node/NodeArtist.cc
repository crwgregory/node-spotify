#include "NodeArtist.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"

NodeArtist::NodeArtist(std::unique_ptr<Artist> _artist) : artist(std::move(_artist)) {
  artist->nodeObject = this;
}

NodeArtist::~NodeArtist() {
  if(artist->nodeObject == this) {
    artist->nodeObject = nullptr;
  }
}

NAN_GETTER(NodeArtist::getName) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(Nan::New<String>(nodeArtist->artist->name().c_str()));
}

NAN_GETTER(NodeArtist::getLink) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(Nan::New<String>(nodeArtist->artist->link().c_str()));
}

NAN_METHOD(NodeArtist::browse) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    nodeArtist->makePersistent();
    sp_artistbrowse_type artistbrowseType = static_cast<sp_artistbrowse_type>(args[0]->ToNumber()->IntegerValue());
    nodeArtist->browseCompleteCallback = std::unique_ptr<Nan::Callback>(new Nan::Callback(args[1].As<Function>()));

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = NanObjectWrapHandle(nodeArtist);
    nodeArtistV8->SetAccessor(Nan::New<String>("tracks"), getTracks);
    nodeArtistV8->SetAccessor(Nan::New<String>("tophitTracks"), getTophitTracks);
    nodeArtistV8->SetAccessor(Nan::New<String>("albums"), getAlbums);
    nodeArtistV8->SetAccessor(Nan::New<String>("similarArtists"), getSimilarArtists);
    nodeArtistV8->SetAccessor(Nan::New<String>("biography"), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse(artistbrowseType);
  } else {
    nodeArtist->callBrowseComplete();
  }
  NanReturnUndefined();
}

NAN_GETTER(NodeArtist::getTracks) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeArtist->artist->tracks();
  Local<Array> nodeTracks = Nan::New<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTracks);
}

NAN_GETTER(NodeArtist::getTophitTracks) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::shared_ptr<Track>> tophitTracks = nodeArtist->artist->tophitTracks();
  Local<Array> nodeTophitTracks = Nan::New<Array>(tophitTracks.size());
  for(int i = 0; i < (int)tophitTracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tophitTracks[i]);
    nodeTophitTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTophitTracks);
}

NAN_GETTER(NodeArtist::getAlbums) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::unique_ptr<Album>> albums = nodeArtist->artist->albums();
  Local<Array> nodeAlbums = Nan::New<Array>(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(std::move(albums[i]));
    nodeAlbums->Set(Nan::New<Number>(i), nodeAlbum->createInstance());
  }
  NanReturnValue(nodeAlbums);
}

NAN_GETTER(NodeArtist::getSimilarArtists) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::unique_ptr<Artist>> similarArtists = nodeArtist->artist->similarArtists();
  Local<Array> nodeSimilarArtists = Nan::New<Array>(similarArtists.size());
  for(int i = 0; i < (int)similarArtists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(std::move(similarArtists[i]));
    nodeSimilarArtists->Set(Nan::New<Number>(i), nodeArtist->createInstance());
  }
  NanReturnValue(nodeSimilarArtists);
}

NAN_GETTER(NodeArtist::getBiography) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::string biography = nodeArtist->artist->biography();
  NanReturnValue(Nan::New<String>(biography.c_str()));
}

NAN_GETTER(NodeArtist::isLoaded) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(Nan::New<Boolean>(nodeArtist->artist->isLoaded()));
}

void NodeArtist::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name"), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  NanAssignPersistent(NodeArtist::constructorTemplate, constructorTemplate);
}
