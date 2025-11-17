cmake_minimum_required(VERSION 3.8)
project(M40EP)

set(CMAKE_CXX_STANDARD 20)
include_directories(include/imgui)
include_directories(include/imgui/backends)
include_directories(include/glfw/include)

link_directories(include/glfw/lib)

add_executable(M40EP
        paymentPortal.cpp
        Main.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)

add_executable(Tester
        paymentPortal.cpp
        Tester.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)

add_executable(ImGuiTest
        paymentPortal.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
        Base.cpp
        imGuiTest.cpp

        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp

        include/imgui/backends/imgui_impl_glfw.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(ImGuiTest
        glfw3
        opengl32
)