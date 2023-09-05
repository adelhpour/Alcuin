#include "negui_scene_mode_element_base.h"

// MySceneModeElementBase

MySceneModeElementBase::MySceneModeElementBase() {

}

void MySceneModeElementBase::setSceneMode(const SceneMode& sceneMode) {
    _sceneMode = sceneMode;
}

void MySceneModeElementBase::setSceneMode(const QString& sceneMode) {
    setSceneMode(getModeAsSceneModeFromString(sceneMode));
}

void MySceneModeElementBase::setAlternativeSimilarSceneMode(const SceneMode& alternativeSimilarSceneMode) {
    _alternativeSimilarSceneMode = alternativeSimilarSceneMode;
}

void MySceneModeElementBase::setAlternativeSimilarSceneMode(const QString& alternativeSimilarSceneMode) {
    setAlternativeSimilarSceneMode(getModeAsSceneModeFromString(alternativeSimilarSceneMode));
}

MySceneModeElementBase::SceneMode MySceneModeElementBase::getSceneMode() {
    return _sceneMode;
}

MySceneModeElementBase::SceneMode MySceneModeElementBase::getAlternativeSimilarSceneMode() {
    return _alternativeSimilarSceneMode;
}

const QString MySceneModeElementBase::getSceneModeAsString() {
    return getModeAsStringFromSceneMode(getSceneMode());
}

const QString MySceneModeElementBase::getAlternativeSimilarSceneModeAsString() {
    return getModeAsStringFromSceneMode(getAlternativeSimilarSceneMode());
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

void MySceneModeElementBase::enableDisplayFeatureMenuMode() {
    setSceneMode(DISPLAY_FEATURE_MENU_MODE);
}

const MySceneModeElementBase::SceneMode MySceneModeElementBase::getModeAsSceneModeFromString(const QString& mode) {
    if (mode == "Normal")
        return NORMAL_MODE;
    else if (mode == "Add_Node")
        return ADD_NODE_MODE;
    else if (mode == "Add_Edge")
        return ADD_EDGE_MODE;
    else if (mode == "Select")
        return SELECT_MODE;
    else if (mode == "Select_Node")
        return SELECT_NODE_MODE;
    else if (mode == "Select_Edge")
        return SELECT_EDGE_MODE;
    else if (mode == "Display_Feature_Menu")
        return DISPLAY_FEATURE_MENU_MODE;

    return NONE_MODE;
}

const QString MySceneModeElementBase::getModeAsStringFromSceneMode(const SceneMode& mode) {
    if (mode == NORMAL_MODE)
        return "Normal";
    else if (mode == ADD_NODE_MODE)
        return "Add_Node";
    else if (mode == ADD_EDGE_MODE)
        return "Add_Edge";
    else if (mode == SELECT_MODE)
        return "Select";
    else if (mode == SELECT_NODE_MODE)
        return "Select_Node";
    else if (mode == SELECT_EDGE_MODE)
        return "Select_Edge";
    else if (mode == DISPLAY_FEATURE_MENU_MODE)
        return "Display_Feature_Menu";

    return "";
}