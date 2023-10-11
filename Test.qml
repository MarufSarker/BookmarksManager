import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Bookmarks Manager")
    minimumHeight: 50
    minimumWidth: 50

    Component.onCompleted: {
        listModel.goHome()
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: componentBookmarksList
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
                    onClicked: listModel.goHome()
                }
            }
            ToolButton {
                id: toolbuttonDelete
                text: qsTr("Delete")
                font.bold: true
                visible: false
                onClicked: { componentDeleteBookmarks.createObject(mainWindow).open() }
                function _setVisibility() { visible = listModel.selectHasSelection() }
            }
//            ToolButton {
//                id: toolbuttonCut
//                text: qsTr("Cut")
//                font.bold: true
//                visible: false
//                onClicked: {
//                    for (let i = 0; i < listSelections.selectedIndexes.length; ++i) {
//                        let idx = listSelections.selectedIndexes[i]
//                        if (!idx.valid)
//                            continue
//                        let dt = listModel.getMap(idx.row)
//                        cutIds.push(dt.identifier)
//                    }
//                }
//            }
//            ToolButton {
//                id: toolbuttonPaste
//                text: qsTr("Paste")
//                font.bold: true
//                visible: false
//                onClicked: { }
//            }
            ToolButton {
                text: qsTr("Add")
                font.bold: true
                onClicked: stack.push(componentAddBookmark)
            }
            ToolButton {
                text: qsTr("Search")
                font.bold: true
                onClicked: {
                    footerToolbar.visible = !footerToolbar.visible
                    if (footerToolbar.visible)
                        searchField.forceActiveFocus()
                }
            }
        }
    }

    footer: ToolBar {
        id: footerToolbar
        visible: false
        RowLayout {
            anchors.fill: parent
            focus: true
            Button {
                text: qsTr("Close")
                Layout.alignment: Qt.AlignRight
                Layout.margins: 2
                onClicked: {
                    if (search.__searched) {
                        search.__searched = false
                        listModel.goRefresh()
                    }
                    footerToolbar.visible = false
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
            }
            Button {
                id: search
                text: qsTr("Search")
                Layout.alignment: Qt.AlignRight
                Layout.margins: 2
                property bool __searched: false
                onClicked: {
                    let txt = searchField.text
                    if (!txt || txt.length <= 0)
                        return
                    search.__searched = true
                    listModel.selectIntoModel(txt)
                }
            }
        }
    }

    Component {
        id: listDelegate
        Rectangle {
            width: parent ? parent.width : 0
            implicitHeight: childrenRect ? childrenRect.height : 0
            color: {
                if (model.selected)
                    return "#1C1C1C";
                else if (model.cut)
                    return "#1A1A1A"
                return "transparent"
            }
            border.color: "#FFFFFF"
            border.width: 1
            function _selectToggle() {
                listModel.selectToggle(model.index)
                toolbuttonDelete._setVisibility()
            }
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
                        onClicked: _selectToggle()
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
                        implicitWidth: titleId.width + urlId.width
                        implicitHeight: titleId.height + urlId.height
                        onClicked: {
                            if (listModel.selectHasSelection())
                                _selectToggle()
                            else if (model.type === "CONTAINER")
                                listModel.goInto(model.identifier)
                        }
                    }
                }
                ToolButton {
                    text: qsTr(":")
                    font.bold: true
                    Layout.rightMargin: 5
                    visible: model.title !== "../"
                    onClicked: bookmarkOptions.open()
                    Menu {
                        id: bookmarkOptions
                        closePolicy: Menu.CloseOnPressOutside | Menu.CloseOnEscape
                        MenuItem {
                            text: qsTr("Edit")
                            onClicked: stack.push(componentEditBookmark, {"editingModel": model})
                        }
                    }
                }
            }
        }
    }

    Component {
        id: componentBookmarksList
        ListView {
            id: bookmarksList
            model: listModel
            delegate: listDelegate
            clip: true
            spacing: 5
            visible: true
            ScrollBar.vertical: ScrollBar {}
        }
    }

    component LabeledTextEdit: ColumnLayout {
        property alias label: _label.text
        property alias value: _text.text
        Layout.fillWidth: true
        Label {
            id: _label
            text: "Label"
            color: "#B0B0B0"
            font.italic: true
            Layout.fillWidth: true
            Layout.margins: 0
            Layout.leftMargin: 2
            Layout.rightMargin: 2
        }
        TextEdit {
            id: _text
            text: ""
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

    Component {
        id: componentEditBookmark
        ScrollView {
            property var editingModel: null
            contentWidth: parent ? parent.width : 0
            ColumnLayout {
                width: parent.width
                RowLayout {
                    width: parent.width
                    Button {
                        id: editOk
                        text: qsTr("Edit")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: {
                            editInfo.text = ""
                            if (!editingModel) {
                                editInfo.text = "Editing Data Not Found"
                                return
                            }
                            let bm = {
                                "identifier": editingModel.identifier,
                                "url": editUrl.value,
                                "title": editTitle.value,
                                "note": editNote.value,
                            }
                            let res = listModel.updateBookmarks([bm])
                            if (res) {
                                editInfo.text = "Updated"
                                // listModel.goInto(editingModel.container)
                                listModel.goRefresh()
                                stack.pop()
                            } else
                                editInfo.text = "Error Updating"
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
                    color: "#FFFFFF"
                    font.bold: true
                    Layout.margins: 5
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Qt.TextWrapAnywhere
                }
                LabeledTextEdit {
                    id: editTitle
                    label: "Title"
                    value: editingModel ? editingModel.title : null
                }
                LabeledTextEdit {
                    id: editUrl
                    label: "URL"
                    value: editingModel ? editingModel.url : null
                }
                LabeledTextEdit {
                    id: editNote
                    label: "Note"
                    value: editingModel ? editingModel.note : null
                }
            }
        }
    }

    Component {
        id: componentAddBookmark
        ScrollView {
            contentWidth: parent ? parent.width : 0
            ColumnLayout {
                width: parent.width
                RowLayout {
                    width: parent.width
                    Button {
                        id: addOk
                        text: qsTr("Add")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: {
                            addInfo.text = ""
                            let bm = {
                                "url": addViewUrl.value,
                                "title": addViewTitle.value,
                                "note": addViewNote.value,
                                "type": addViewType.currentValue,
                                "container": listModel.currentContainer(),
                            }
                            if (bm.type !== "URL" && bm.type !== "CONTAINER") {
                                addInfo.text = "Select Type"
                                return
                            }
                            if (bm.type === "URL" && (bm.url === null || bm.url === "")) {
                                addInfo.text = "Add URL"
                                return
                            }
                            if (bm.type === "CONTAINER" && (bm.title === null || bm.title === "")) {
                                addInfo.text = "Add Title"
                                return
                            }
                            let res = listModel.insertBookmarks([bm])
                            if (res) {
                                addInfo.text = "Added"
                                listModel.goRefresh()
                                stack.pop()
                            } else
                                addInfo.text = "Error Adding"
                        }
                    }
                    Button {
                        id: addCancel
                        text: qsTr("Close")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: stack.pop()
                    }
                }
                Text {
                    id: addInfo
                    text: ""
                    color: "#FFFFFF"
                    font.bold: true
                    Layout.margins: 5
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Qt.TextWrapAnywhere
                }
                RowLayout {
                    width: parent.width
                    Label {
                        text: "Type"
                        color: "#B0B0B0"
                        font.italic: true
                        Layout.fillWidth: false
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                    ComboBox {
                        id: addViewType
                        currentIndex: 0
                        textRole: "text"
                        valueRole: "value"
                        model: ListModel {
                            ListElement {text: "URL"; value: "URL"}
                            ListElement {text: "Container"; value: "CONTAINER"}
                        }
                        Layout.fillWidth: true
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                }
                LabeledTextEdit {
                    id: addViewTitle
                    label: "Title"
                    Layout.fillWidth: true
                }
                LabeledTextEdit {
                    id: addViewUrl
                    label: "URL"
                    Layout.fillWidth: true
                }
                LabeledTextEdit {
                    id: addViewNote
                    label: "Note"
                    Layout.fillWidth: true
                }
            }
        }
    }

    Component {
        id: componentDeleteBookmarks
        Dialog {
            property var __deleteModel: ListModel {}
            title: qsTr("Remove Bookmarks")
            width: parent ? (parent.width * 0.8) : 0
            height: {
                let ch = contentChildren.height
                let ph = parent ? (parent.height * 0.8) : 0
                if (ch < ph)
                    return ph
                else
                    return ch
            }
            padding: 10
            margins: 10
            x: parent ? ((parent.width - width) / 2) : 0
            y: parent ? ((parent.height - height) / 2) : 0
            modal: true
            closePolicy: Dialog.NoAutoClose
            standardButtons: Dialog.Ok | Dialog.Cancel
            onAccepted: {
                let ids = []
                for (let i = 0; i < __deleteModel.count; ++i)
                    ids.push(__deleteModel.get(i).identifier)
                let res = listModel.deleteBookmarks(ids)
                if (res)
                    listModel.goRefresh()
                destroy()
            }
            onRejected: destroy()
            ScrollView {
                width: parent ? parent.width : 0
                height: parent ? parent.height : 0
                ListView {
                    clip: true
                    spacing: 5
                    model: __deleteModel
                    Component.onCompleted: {
                        __deleteModel.clear()
                        let sels = listModel.selectGetSelections()
                        for (let i = 0; i < sels.length; ++i)
                            __deleteModel.append(sels[i])
                    }
                    delegate: Component {
                        Rectangle {
                            border.color: "#FFFFFF"
                            border.width: 1
                            color: "transparent"
                            width: parent ? parent.width : 0
                            height: childrenRect.height
                            ColumnLayout {
                                width: parent ? parent.width : 0
                                anchors.margins: 3
                                Label {
                                    text: title
                                    Layout.fillWidth: true
                                    font.bold: true
                                    color: "#FFFFFF"
                                    wrapMode: Text.WordWrap
                                    padding: 2
                                }
                                Label {
                                    text: url
                                    Layout.fillWidth: true
                                    color: "#FFFFFF"
                                    wrapMode: Text.WordWrap
                                    padding: 2
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
