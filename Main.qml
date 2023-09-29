import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

//import Bookmark

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Bookmarks Manager")
    minimumHeight: 50
    minimumWidth: 50

    property list<int> listSelections: []
    property list<string> listContainersStack: []
    property var listCurrentModel: null
    property var listEditingModel: null

    function addToSelections(model) {
        let idx = listSelections.indexOf(model.index)
        if (idx >= 0)
            listSelections.splice(idx, 1)
        else
        {
            listSelections.push(model.index)
            listCurrentModel = model
        }
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

    function refreshListView(model) {
        clearSelections()
        if (model.container)
            bookmarkListModel.selectFromContainerIntoModel(model.container)
    }

    function upListView() {
        if (listContainersStack.length <= 0)
            return
        clearSelections()
        bookmarkListModel.selectFromContainerIntoModel(listContainersStack.pop())
    }

    function loadListView(model) {
        if (model.type === "CONTAINER") {
            listContainersStack.push(model.container)
            bookmarkListModel.selectFromContainerIntoModel(model.identifier)
        }
    }

    function selectOrLoadListView(model) {
        if (listSelections.length > 0) {
            addToSelections(model)
        } else {
            loadListView(model)
        }
    }

    function searchIntoListView(text) {
        bookmarkListModel.selectIntoModel(text)
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: "Boomarks"
                color: "#FFFFFF"
                font.bold: true
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
                onClicked: {
                    if (listSelections.length !== 1)
                        return
                    listEditingModel = listCurrentModel
                    stack.push(editBookmarkComponent)
                    // editBookmarkView._model = listCurrentModel
                    // bookmarksList.visible = false
                    // editBookmarkView.visible = true
                }
            }
            ToolButton {
                text: qsTr("U")
                font.bold: true
                visible: listContainersStack.length > 0
                onClicked: upListView()
            }
            ToolButton {
                text: qsTr("+")
                font.bold: true
                onClicked: {
                    // TODO
                }
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
                            addToSelections(model)
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
                            selectOrLoadListView(model)
                            __active = isSelected(model.index)
                        }
                    }
                }

                ToolButton {
                    text: qsTr(":")
                    font.bold: true
                    Layout.rightMargin: 5
                    onClicked: {
                        bookmarkOptions.open()
                    }

                    Menu {
                        id: bookmarkOptions
                        closePolicy: Menu.CloseOnPressOutside | Menu.CloseOnEscape
                        MenuItem {
                            text: qsTr("Edit")
                            onClicked: {
                                listEditingModel = model
                                stack.push(editBookmarkComponent)
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: bookmarksListComponent

        ListView {
            id: bookmarksList
            anchors.fill: parent
            model: bookmarkListModel
            delegate: listDelegate
            clip: true
            spacing: 5
            visible: true
            ScrollBar.vertical: ScrollBar {}
        }
    }

    Component {
        id: editBookmarkComponent

        ScrollView {
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                ColumnLayout {
                    anchors.fill: parent
                    RowLayout {
                        Layout.fillWidth: true
                        Button {
                            id: editOk
                            text: qsTr("Edit")
                            Layout.fillWidth: true
                            Layout.margins: 5
                            onClicked: {
                                editInfo.text = ""
                                let bm = {
                                    "identifier": listEditingModel.identifier,
                                    "url": editListView.model.get(1).value,
                                    "title": editListView.model.get(0).value,
                                    "note": editListView.model.get(2).value,
                                }
                                let res = bookmarkListModel.updateBookmarks([bm])
                                if (res) {
                                    editInfo.text = "Updated"
                                    refreshListView(listEditingModel)
                                    stack.pop()
                                } else
                                    editInfo.text = "Error Updating"

//                                console.log(res)
//                                console.log(res, bm.identifier, bm.url, bm.title, bm.note)

                            }
                        }
                        Button {
                            id: editCancel
                            text: qsTr("Close")
                            Layout.fillWidth: true
                            Layout.margins: 5
                            onClicked: stack.pop()
                        }
                    }
                    Text {
                        id: editInfo
                        text: ""
                        visible: true
                        color: "#FFFFFF"
                        font.bold: true
                        wrapMode: Qt.TextWrapAnywhere
                        Layout.fillWidth: true
                        Layout.margins: 3
                        Layout.alignment: Qt.AlignCenter
                    }
                    ListView {
                        id: editListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        spacing: 5
                        model: ListModel {}
                        Component.onCompleted: {
                            model.append({"_id": "titleId", "name": "Title", "value": listEditingModel ? listEditingModel.title : ""})
                            model.append({"_id": "urlId", "name": "URL", "value": listEditingModel ? listEditingModel.url : ""})
                            model.append({"_id": "noteId", "name": "Note", "value": listEditingModel ? listEditingModel.note : ""})
                        }
                        delegate: Component {
                            ColumnLayout {
                                width: parent.width
                                Label {
                                    text: name
                                    color: "#B0B0B0"
                                    font.italic: true
                                    Layout.margins: 0
                                    Layout.leftMargin: 2
                                    Layout.rightMargin: 2
                                }
                                TextEdit {
                                    id: _id
                                    text: value
                                    color: "#FFFFFF"
                                    font.bold: true
                                    readOnly: false
                                    selectByMouse: true
                                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                    Layout.fillWidth: true
                                    Layout.margins: 0
                                    Layout.leftMargin: 5
                                    Layout.rightMargin: 5
                                    Layout.bottomMargin: 2
                                    onTextChanged: value = text
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: bookmarksListComponent
    }
}
