#ifndef DLIB_RENDER_DeTECTIONS_H_
#define DLIB_RENDER_DeTECTIONS_H_

#include "full_object_detection.h"
#include "../gui_widgets.h"
#include "render_detections_abstract.h"
#include <vector>

namespace dlib
{
    inline std::vector<image_window::overlay_line> render_detections (
        const std::vector<full_object_detection>& dets,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<image_window::overlay_line> lines;
        for (unsigned long i = 0; i < dets.size(); ++i)
        {
            const full_object_detection& d = dets[i];

            for (unsigned long i = 1; i <= d.num_parts() - 1; ++i)
                lines.push_back(image_window::overlay_line(d.part(i), d.part(i-1), color));
            lines.push_back(image_window::overlay_line(d.part(0), d.part(d.num_parts() - 1), color));
        }
        return lines;
    }

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_line> render_detections (
        const full_object_detection& det,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<full_object_detection> dets;
        dets.push_back(det);
        return render_detections(dets, color);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_RENDER_DeTECTIONS_H_

