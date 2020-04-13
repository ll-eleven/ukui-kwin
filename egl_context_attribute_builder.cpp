/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2017 Martin Flöser <mgraesslin@kde.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#include "egl_context_attribute_builder.h"
#include <epoxy/egl.h>

namespace KWin
{
std::vector<int> EglContextAttributeBuilder::build() const
{
    std::vector<int> attribs;
    if (isVersionRequested()) {
        attribs.emplace_back(EGL_CONTEXT_MAJOR_VERSION_KHR);
        attribs.emplace_back(majorVersion());
        attribs.emplace_back(EGL_CONTEXT_MINOR_VERSION_KHR);
        attribs.emplace_back(minorVersion());
    }
    int contextFlags = 0;
    if (isRobust()) {
        attribs.emplace_back(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR);
        attribs.emplace_back(EGL_LOSE_CONTEXT_ON_RESET_KHR);
        contextFlags |= EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR;
    }
    if (isForwardCompatible()) {
        contextFlags |= EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR;
    }
    if (contextFlags != 0) {
        attribs.emplace_back(EGL_CONTEXT_FLAGS_KHR);
        attribs.emplace_back(contextFlags);
    }
    if (isCoreProfile() || isCompatibilityProfile()) {
        attribs.emplace_back(EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR);
        if (isCoreProfile()) {
            attribs.emplace_back(EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR);
        } else if (isCompatibilityProfile()) {
            attribs.emplace_back(EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR);
        }
    }
    if (isHighPriority()) {
        attribs.emplace_back(EGL_CONTEXT_PRIORITY_LEVEL_IMG);
        attribs.emplace_back(EGL_CONTEXT_PRIORITY_HIGH_IMG);
    }
    attribs.emplace_back(EGL_NONE);
    return attribs;
}

std::vector<int> EglOpenGLESContextAttributeBuilder::build() const
{
    std::vector<int> attribs;
    attribs.emplace_back(EGL_CONTEXT_CLIENT_VERSION);
    attribs.emplace_back(majorVersion());
    if (isRobust()) {
        attribs.emplace_back(EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT);
        attribs.emplace_back(EGL_TRUE);
        attribs.emplace_back(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT);
        attribs.emplace_back(EGL_LOSE_CONTEXT_ON_RESET_EXT);
    }
    if (isHighPriority()) {
        attribs.emplace_back(EGL_CONTEXT_PRIORITY_LEVEL_IMG);
        attribs.emplace_back(EGL_CONTEXT_PRIORITY_HIGH_IMG);
    }
    attribs.emplace_back(EGL_NONE);
    return attribs;
}

}
