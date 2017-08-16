//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_PCH_HPP
#define OMICRONRENDER_PCH_HPP

#include <glad/glad.h>
#include <string>
#include <script/ScriptHost.hpp>

#define OCULUS_APP_ID "1481421391919456"

#define DEBUG_PRINT(code) printf("%s\n", #code); code
//#define DEBUG_PRINT(code) code

#ifndef DELTA_TIMER
#define DELTA_TIMER

#define DELTA_TIMER_DEC \
float delta = 0.f; \
float current = 0.f; \
float last = 0.f;

#define DELTA_TIMER_DEF \
current = glfwGetTime(); \
delta = current - last; \
last = current;

#endif //DELTA_TIMER


#ifndef CLEAR_PTR
#define CLEAR_PTR(ptr) \
delete ptr; \
ptr = nullptr;
#endif

namespace Omicron {

    int GetDrawCalls();
    void IncrementDrawCalls();
    void ResetDrawCalls();

}

//#define DrawArrays(mode, first, count) Omicron::IncrementDrawCalls(); glDrawArrays(mode, first, count);
//#define DrawElements(mode, count, type, indices) Omicron::IncrementDrawCalls(); glDrawElements(mode, count, type, indices);
//#define SwapBuffers(win) Omicron::ResetDrawCalls(); glfwSwapBuffers(win);
//
//#define DrawArrays(mode, first, count) glDrawArrays(mode, first, count);
//#define DrawElements(mode, count, type, indices) glDrawElements(mode, count, type, indices);
//#define SwapBuffers(win) glfwSwapBuffers(win);

#endif //OMICRONRENDER_PCH_HPP
