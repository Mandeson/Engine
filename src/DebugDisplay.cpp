#include "DebugDisplay.hpp"
#include "util/Vector.hpp"
#include <format>

DebugDisplay::DebugDisplay(ThreadPool &thread_pool, Font &font, float ui_scale)
        : ui_scale_(ui_scale), text_(thread_pool, font, ui_scale * 18.0f), num_frames_(0), max_frame_time_(0.0),
        report_timer_(0.0) { }

void DebugDisplay::timeStep(double time) {
    num_frames_++;
    if (time > max_frame_time_)
        max_frame_time_ = time;
    report_timer_ += time;

    if (report_timer_ > kReportDelay) {
        double avg_fps = num_frames_ / report_timer_;
        text_.setString(std::format("AVG FPS: {:.1f}  Max frame time: {:.4f}", avg_fps, max_frame_time_));
        num_frames_ = 0;
        max_frame_time_ = 0.0;
        report_timer_ = 0.0;
    }
}

void DebugDisplay::render(TextRenderer &text_renderer) {
    text_renderer.render(text_, Vector2f{20 * ui_scale_, 20 * ui_scale_});
}
