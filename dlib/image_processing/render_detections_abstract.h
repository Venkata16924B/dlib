#undef DLIB_RENDER_DeTECTIONS_ABSTRACT_H_
#ifdef DLIB_RENDER_DeTECTIONS_ABSTRACT_H_

#include "full_object_detection_abstract.h"
#include "../gui_widgets.h"

namespace dlib
{

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_line> render_detections (
        const std::vector<full_object_detection>& dets,
        const rgb_pixel color = rgb_pixel(0,255,0)
    );

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_line> render_detections (
        const full_object_detection& det,
        const rgb_pixel color = rgb_pixel(0,255,0)
    );

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_RENDER_DeTECTIONS_ABSTRACT_H_


