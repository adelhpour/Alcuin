class NodeInterface {
public:
    virtual ~NodeInterface() = default;
    virtual QString node(const QString &message) = 0;
    virtual void loadScript(const QString &message) = 0;
};

#define NodeInterface_iid "org.sys-bio.networkeditorgui.nodeinterface/1.0"

Q_DECLARE_INTERFACE(NodeInterface, NodeInterface_iid)

