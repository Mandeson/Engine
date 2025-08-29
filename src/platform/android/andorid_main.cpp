#include <android/asset_manager.h>
#include <iostream>
#include "Engine.hpp"

AAssetManager *g_asset_manager;

AAssetManager *getAssetManager() {
    return  g_asset_manager;
}

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.h>

bool motionEventFilter(const GameActivityMotionEvent *motionEvent) {
    auto source_class = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (source_class == AINPUT_SOURCE_CLASS_POINTER || source_class == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app *app) {
    g_asset_manager = app->activity->assetManager;
    android_app_set_motion_event_filter(app, motionEventFilter);
    auto *engine = new EngineImpl(app);

    engine->gameLoop();

    delete engine;
}

}
