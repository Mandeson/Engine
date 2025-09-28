#pragma once

#include "ThreadPool.hpp"
#include "renderer/TextRenderer.hpp"
#include "text/Font.hpp"
#include "text/Text.hpp"

class DebugDisplay {
public:
    DebugDisplay(ThreadPool &thread_pool, Font &font, float ui_scale);
    void timeStep(double time);
    void render(TextRenderer &text_renderer);
private:
    float ui_scale_;
    Text text_;
    int num_frames_;
    double max_frame_time_;
    double report_timer_;

    constexpr static float kReportDelay = 1.0;
};
