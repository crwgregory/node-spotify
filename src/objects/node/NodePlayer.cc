#include "NodePlayer.h"
#include "NodeTrack.h"
#include "../../callbacks/SessionCallbacks.h"
#include "../../exceptions.h"
#include "../../utils/V8Utils.h"

NodePlayer::NodePlayer(std::shared_ptr<Player> _player) : player(_player) {}

NodePlayer::~NodePlayer() {

}

NodePlayer::NodePlayer(const NodePlayer& other) {

}

NAN_METHOD(NodePlayer::pause) {
  NanScope();
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->pause();
  NanReturnUndefined();
}

NAN_METHOD(NodePlayer::stop) {
  NanScope();
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->stop();
  NanReturnUndefined();
}

NAN_METHOD(NodePlayer::resume) {
  NanScope();
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->resume();
  NanReturnUndefined();
}

NAN_METHOD(NodePlayer::play) {
  NanScope();
  if(args.Length() < 1) {
    return Nan::ThrowError("play needs a track as its first argument.");
  }
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args[0]->ToObject());
  try {
    nodePlayer->player->play(nodeTrack->track);
  } catch (const TrackNotPlayableException& e) {
    return Nan::ThrowError("Track not playable");
  }
#ifndef NODE_SPOTIFY_NATIVE_SOUND
  catch (const NoAudioHandlerException& e) {
    return Nan::ThrowError("No audio handler registered. Use spotify.useNodejsAudio().");
  }
#endif

  NanReturnUndefined();
}

NAN_METHOD(NodePlayer::seek) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return Nan::ThrowError("seek needs an integer as its first argument.");
  }
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  int second = args[0]->ToInteger()->Value();
  nodePlayer->player->seek(second);
  NanReturnUndefined();
}

NAN_GETTER(NodePlayer::getCurrentSecond) {
  NanScope();
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  NanReturnValue(Nan::New<Integer>(nodePlayer->player->currentSecond));
}

NAN_METHOD(NodePlayer::on) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsObject()) {
    return Nan::ThrowError("on needs an object as its first argument.");
  }
  Handle<Object> callbacks = args[0]->ToObject();
  Handle<String> endOfTrackKey = Nan::New<String>("endOfTrack").ToLocalChecked();
  SessionCallbacks::endOfTrackCallback = V8Utils::getFunctionFromObject(callbacks, endOfTrackKey);
  NanReturnUndefined();
}

NAN_METHOD(NodePlayer::off) {
  NanScope();
  SessionCallbacks::endOfTrackCallback = std::unique_ptr<Nan::Callback>(new Nan::Callback());
  NanReturnUndefined();
}

void NodePlayer::init() {
  NanScope();
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>();
  constructorTemplate->SetClassName(Nan::New<String>("Player").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);

  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "seek", seek);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("currentSecond").ToLocalChecked(), &getCurrentSecond);
  NanAssignPersistent(NodePlayer::constructorTemplate, constructorTemplate);
}
