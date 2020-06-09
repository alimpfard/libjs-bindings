/*
 * Copyright (c) 2020, Linus Groh <mail@linusgroh.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Application.h"
#include "../GlobalObject.h"

namespace Bindings {
namespace LibGUI {

// BEGIN_OBJECT
Application* Application::create(JS::GlobalObject& global_object)
{
    return global_object.heap().allocate<Application>(*static_cast<GlobalObject&>(global_object).gui_application_prototype());
}

Application::Application(JS::Object& prototype)
    : Object(&prototype)
{
    set_prototype(&prototype);
}
// END_OBJECT

// BEGIN_CONSTRUCTOR
ApplicationConstructor::ApplicationConstructor()
    : JS::NativeFunction(*interpreter().global_object().function_prototype())
{
    define_property("length", JS::Value(0), JS::Attribute::Configurable);

    u8 attr = JS::Attribute::Writable | JS::Attribute::Configurable;
    define_native_function("the", the, 0, attr);
}

JS::Value ApplicationConstructor::call(JS::Interpreter& interpreter)
{
    return construct(interpreter);
}

JS::Value ApplicationConstructor::construct(JS::Interpreter& interpreter)
{
    return Application::create(interpreter.global_object());
}

JS::Value ApplicationConstructor::the(JS::Interpreter& interpreter)
{
    return Application::create(interpreter.global_object());
}
// END_CONSTRUCTOR

// BEGIN_PROTOTYPE
ApplicationPrototype::ApplicationPrototype()
    : Object(interpreter().global_object().object_prototype())
{
    u8 attr = JS::Attribute::Writable | JS::Attribute::Configurable;
    define_native_function("exec", exec, 0, attr);
}

static GUI::Application* app_from(JS::Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value(interpreter.global_object()).to_object(interpreter);
    if (!this_object)
        return nullptr;
    if (StringView("Application") != this_object->class_name()) {
        interpreter.throw_exception<JS::TypeError>("Not an Application object");
        return nullptr;
    }
    return &GUI::Application::the();
}

JS::Value ApplicationPrototype::exec(JS::Interpreter& interpreter)
{
    auto* app = app_from(interpreter);
    if (!app)
        return {};
    return JS::Value(app->exec());
}
// END_PROTOTYPE

}
}
