#include "negui_scene_mode_element_base.h"

// MySceneModeElementBase

MySceneModeElementBase::MySceneModeElementBase() {

}

void MySceneModeElementBase::setSceneMode(SceneMode sceneMode) {
    _sceneMode = sceneMode;
}

MySceneModeElementBase::SceneMode MySceneModeElementBase::getSceneMode() {
    return _sceneMode;
}

const QString MySceneModeElementBase::getSceneModeAsString() {
    if (_sceneMode == NORMAL_MODE)
        return "Normal";
    else if (_sceneMode == ADD_NODE_MODE)
        return "Add_Node";
    else if (_sceneMode == ADD_EDGE_MODE)
        return "Add_Edge";
    else if (_sceneMode == SELECT_MODE)
        return "Select";
    else if (_sceneMode == SELECT_NODE_MODE)
        return "Select_Node";
    else if (_sceneMode == SELECT_EDGE_MODE)
        return "Select_Edge";
    else if (_sceneMode == REMOVE_MODE)
        return "Remove";

    return "";
}

void MySceneModeElementBase::enableNormalMode() {
    setSceneMode(NORMAL_MODE);
}

void MySceneModeElementBase::enableAddNodeMode() {
    setSceneMode(ADD_NODE_MODE);
}

void MySceneModeElementBase::enableAddEdgeMode() {
    setSceneMode(ADD_EDGE_MODE);
}

void MySceneModeElementBase::enableSelectMode() {
    setSceneMode(SELECT_MODE);
}

void MySceneModeElementBase::enableSelectNodeMode() {
    setSceneMode(SELECT_NODE_MODE);
}

void MySceneModeElementBase::enableSelectEdgeMode() {
    setSceneMode(SELECT_EDGE_MODE);
}

void MySceneModeElementBase::enableRemoveMode() {
    setSceneMode(REMOVE_MODE);
}
