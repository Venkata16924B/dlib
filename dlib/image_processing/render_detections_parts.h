#ifndef DLIB_RENDER_DeTECTIONS_PARTS_H_
#define DLIB_RENDER_DeTECTIONS_PARTS_H_

#include "full_object_detection.h"
#include "../gui_widgets.h"
#include <vector>

namespace dlib
{
    inline std::vector<image_window::overlay_circle> render_detections_parts (
        const std::vector<full_object_detection>& dets,
        const rgb_pixel color = rgb_pixel(255,0,0)
    )
    {
        std::vector<image_window::overlay_circle> circles;
        for (unsigned long i = 0; i < dets.size(); ++i)
        {
            const full_object_detection& d = dets[i];
            for (unsigned long i = 0; i < d.num_parts(); ++i)
            {
                circles.push_back(image_window::overlay_circle(d.part(i), 3, color));
            }
        }
        return circles;
    }

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_circle> render_detections_parts (
        const full_object_detection& det,
        const rgb_pixel color = rgb_pixel(255,0,0)
    )
    {
        std::vector<full_object_detection> dets;
        dets.push_back(det);
        return render_detections_parts(dets, color);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_RENDER_DeTECTIONS_PARTS_H_
