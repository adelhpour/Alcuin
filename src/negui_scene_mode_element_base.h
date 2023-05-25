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
        REMOVE_MODE,
    } SceneMode;

    MySceneModeElementBase();

    virtual void setSceneMode(SceneMode mode);

    SceneMode getSceneMode();

    const QString getSceneModeAsString();

    virtual void enableNormalMode();

    virtual void enableAddNodeMode();

    virtual void enableAddEdgeMode();

    virtual void enableSelectMode();

    virtual void enableSelectNodeMode();

    virtual void enableSelectEdgeMode();

    virtual void enableRemoveMode();

protected:
    SceneMode _sceneMode;
};

#endif
