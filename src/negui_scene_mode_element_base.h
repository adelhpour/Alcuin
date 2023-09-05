#ifndef __NEGUI_SCENE_MODE_ELEMENT_BASE_H
#define __NEGUI_SCENE_MODE_ELEMENT_BASE_H

#include <QString>

class MySceneModeElementBase {

public:

    typedef enum {
        NORMAL_MODE,
        ADD_NODE_MODE,
        ADD_EDGE_MODE,
        SELECT_MODE,
        SELECT_NODE_MODE,
        SELECT_EDGE_MODE,
        DISPLAY_FEATURE_MENU_MODE,
        NONE_MODE,
    } SceneMode;

    MySceneModeElementBase();

    virtual void setSceneMode(const SceneMode& sceneMode);

    virtual void setSceneMode(const QString& sceneMode);

    virtual void setAlternativeSimilarSceneMode(const SceneMode& alternativeSimilarSceneMode);

    virtual void setAlternativeSimilarSceneMode(const QString& alternativeSimilarSceneMode);

    SceneMode getSceneMode();

    const QString getSceneModeAsString();

    SceneMode getAlternativeSimilarSceneMode();

    const QString getAlternativeSimilarSceneModeAsString();

    virtual void enableNormalMode();

    virtual void enableAddNodeMode();

    virtual void enableAddEdgeMode();

    virtual void enableSelectMode();

    virtual void enableSelectNodeMode();

    virtual void enableSelectEdgeMode();

    virtual void enableDisplayFeatureMenuMode();

    const SceneMode getModeAsSceneModeFromString(const QString& mode);

    const QString getModeAsStringFromSceneMode(const SceneMode& mode);

protected:
    SceneMode _sceneMode;
    SceneMode _alternativeSimilarSceneMode;
};

#endif
