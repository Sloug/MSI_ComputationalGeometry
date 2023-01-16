set(PROJECT_NAME "${PROJECT_MAIN_NAME}_Aufgabe1")
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE UI_FILES *.ui)

set(INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

QT6_WRAP_UI( UI_HEADERS ${UI_FILES} )

include_directories(${INCLUDE_PATHS})

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}    
)

target_link_options(${PROJECT_NAME} INTERFACE "-lGL -lglut")

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
)
