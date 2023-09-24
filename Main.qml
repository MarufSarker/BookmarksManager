import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import Bookmark

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Bookmarks Manager")
    minimumHeight: 300
    minimumWidth: 300

    property list<int> listSelections: []
    property list<string> listContainersStack: []

    function addToSelections(modelIndex) {
        let idx = listSelections.indexOf(modelIndex)
        if (idx >= 0)
            listSelections.splice(idx, 1)
        else
            listSelections.push(modelIndex)
    }

    function isSelected(modelIndex) {
        return listSelections.indexOf(modelIndex) >= 0;
    }

    function clearContainersStack() {
        while (listContainersStack.length > 0)
            listContainersStack.pop()
    }

    function clearSelections() {
        while (listSelections.length > 0)
            listSelections.pop()
    }

    function resetListView() {
        clearContainersStack()
        bookmarkListModel.selectFromContainerIntoModel("0")
    }

    function upListView() {
        if (listContainersStack.length <= 0)
            return
        clearSelections()
        bookmarkListModel.selectFromContainerIntoModel(listContainersStack.pop())
    }

    function loadListView(modelType, modelContainer, modelIdentifier) {
        if (modelType === "CONTAINER") {
            listContainersStack.push(modelContainer)
            bookmarkListModel.selectFromContainerIntoModel(modelIdentifier)
        }
    }

    function selectOrLoadListView(modelIndex, modelType, modelContainer, modelIdentifier) {
        if (listSelections.length > 0) {
            addToSelections(modelIndex)
        } else {
            loadListView(modelType, modelContainer, modelIdentifier)
        }
    }

    function searchIntoListView(text) {
        bookmarkListModel.selectIntoModel(text)
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
//            ToolButton {
//                text: qsTr("‹")
//                font.bold: true
//                onClicked: console.log("< clicked")
//            }
            Label {
                text: "Boomarks"
                color: "#FFFFFF"
                font.bold: true
//                elide: Label.ElideRight
//                horizontalAlignment: Qt.AlignHCenter
//                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.margins: 5
                MouseArea {
                    anchors.fill: parent
                    onClicked: resetListView()
                }
            }
            ToolButton {
                text: qsTr("E")
                font.bold: true
                visible: listSelections.length === 1
                onClicked: {}
            }
            ToolButton {
                text: qsTr("U")
                font.bold: true
                visible: listContainersStack.length > 0
                onClicked: upListView()
            }
            ToolButton {
                text: qsTr("S")
                font.bold: true
                onClicked: {
                    footerToolbar.visible = !footerToolbar.visible
                    if (footerToolbar.visible)
                        searchField.forceActiveFocus()
                }
            }
        }
    }
    // https://stackoverflow.com/questions/26879266/make-toast-in-android-by-qml
    // https://gist.github.com/jonmcclung/bae669101d17b103e94790341301c129
    // https://doc.qt.io/qt-6/qml-qtquick-dialogs-messagedialog.html
    footer: ToolBar {
        id: footerToolbar
        visible: false
        RowLayout {
            anchors.fill: parent
            focus: true
            Button {
                text: qsTr("X")
                Layout.alignment: Qt.AlignRight
                Layout.margins: 2
                onClicked: {
                    resetListView()
                    footerToolbar.visible = !footerToolbar.visible
                    if (footerToolbar.visible)
                        searchField.forceActiveFocus()
                }
            }
            TextField {
                id: searchField
                focus: true
                font.bold: true
                color: "#FFFFFF"
                placeholderText: qsTr("Search...")
                placeholderTextColor: "#FFFFFF"
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                Layout.margins: 2
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Keys.onReturnPressed: search.clicked()
                Keys.onEnterPressed: search.clicked()

                // TODO: Use this to detect touch enter key code and resolve android enter keypress event
                // Keys.onPressed: (event) => { console.log(event.key) }
            }
            Button {
                id: search
                text: qsTr("Search")
                Layout.alignment: Qt.AlignRight
                Layout.margins: 2
                onClicked: searchIntoListView(searchField.text)
            }
        }
    }

    Component {
        id: listDelegate

        Rectangle {
            readonly property ListView __lv: ListView.view
            property bool __active: false

            width: parent ? parent.width : 0
            implicitHeight: childrenRect ? childrenRect.height : 0
            color: __active ? "#1C1C1C" : "transparent" // "#2A2E32"
            border.color: "#FFFFFF"
            border.width: 1

            RowLayout {
                width: parent.width

                Rectangle {
                    id: iconRect
                    implicitWidth: (parent.width * 0.05) < 32 ? 32 : (parent.width * 0.05)
                    Layout.fillHeight: true
                    color: "transparent"

                    Image {
                        source: {
                            if (model.type === "CONTAINER")
                                return "qrc:/qt/qml/BookmarksManager/images/typeContainer.png"
                            else if (model.type === "URL")
                                return "qrc:/qt/qml/BookmarksManager/images/typeURL.png"
                            else
                                return ""
                        }
                        anchors.fill: parent
                        anchors.margins: 3
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            addToSelections(model.index)
                            __active = isSelected(model.index)
                        }
                    }
                }

                Rectangle {
                    id: infoRect
                    Layout.fillWidth: true
                    implicitHeight: childrenRect.height
                    color: "transparent"

                    ColumnLayout {
                        width: parent.width

                        Label {
                            id: titleId
                            text: title
                            Layout.fillWidth: true
                            font.bold: true
                            color: "#FFFFFF"
                            wrapMode: Text.WordWrap
                            padding: 2
                        }

                        Label {
                            id: urlId
                            text: url
                            Layout.fillWidth: true
                            color: "#FFFFFF"
                            wrapMode: Text.WordWrap
                            padding: 2
                        }

                    }

                    MouseArea {
                        // anchors.fill: parent // causes issue with childrenRect.height
                        implicitWidth: titleId.width + urlId.width
                        implicitHeight: titleId.height + urlId.height
                        onClicked: {
                            selectOrLoadListView(model.index, model.type, model.container, model.identifier)
                            __active = isSelected(model.index)
                        }
                    }
                }
            }
        }
    }

    ListView {
        property list<int> _selections: []
        property list<string> _containersStack: []

        id: bookmarksList
        anchors.fill: parent
        model: bookmarkListModel
        delegate: listDelegate
        clip: true
        spacing: 5
    }
}
