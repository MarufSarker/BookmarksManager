import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
import QtQuick.Controls.Universal

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Bookmarks Manager")
    minimumHeight: 300
    minimumWidth: 300

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Cyan

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
                Layout.leftMargin: 10
                MouseArea {
                    anchors.fill: parent
                    onClicked: listModel.goHome()
                }
            }
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
            ToolButton {
                text: qsTr(":")
                font.bold: true
                Layout.rightMargin: 5
                onClicked: toolbarExtras.open()
                Menu {
                    id: toolbarExtras
                    closePolicy: Menu.CloseOnPressOutside | Menu.CloseOnEscape
                    MenuItem {
                        id: toolbuttonCut
                        text: qsTr("Cut")
                        enabled: false
                        onClicked: listModel.cutSelections()
                        Connections {
                            target: listModel
                            function onSelectionsSizeChanged() {
                                toolbuttonCut.enabled = listModel.selectHasSelection()
                            }
                        }
                    }
                    MenuItem {
                        id: toolbuttonPaste
                        text: qsTr("Paste")
                        enabled: false
                        onClicked: {
                            let res = listModel.cutPaste()
                            if (res)
                                listModel.goRefresh()
                        }
                        Connections {
                            target: listModel
                            function onCutSizeChanged() {
                                toolbuttonPaste.enabled = listModel.cutHasSelection()
                            }
                        }
                    }
                    MenuSeparator {}
                    MenuItem {
                        id: toolbuttonDelete
                        text: qsTr("Delete")
                        enabled: false
                        onClicked: { componentDeleteBookmarks.createObject(mainWindow).open() }
                        Connections {
                            target: listModel
                            function onSelectionsSizeChanged() {
                                toolbuttonDelete.enabled = listModel.selectHasSelection()
                            }
                        }
                    }
                    MenuSeparator {}
                    MenuItem {
                        text: qsTr("Import")
                        onClicked: { stack.push(componentImportBookmarks) }
                    }
                    MenuItem {
                        text: qsTr("Export")
                        onClicked: {
                            exportFolderDialog.open()
                        }
                    }
                    MenuSeparator {}
                    MenuItem {
                        text: qsTr("Vacuum")
                        onClicked: { componentVacuumDatabase.createObject(mainWindow).open() }
                    }
                    MenuSeparator {}
                    MenuItem {
                        text: qsTr("Settings")
                        onClicked: { stack.push(componentSettings) }
                    }
                    MenuSeparator {}
                    MenuItem {
                        text: qsTr("Exit")
                        onClicked: { Qt.quit() }
                    }
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
                placeholderText: qsTr("Search...")
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
                    return "#3C3C3C";
                else if (model.cut)
                    return "#555555"
                return "transparent"
            }
            border.color: "#FFFFFF"
            border.width: 1
            function _selectToggle() {
                if (model.title !== "../")
                    listModel.selectToggle(model.index)
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
                                return "qrc:/assets/images/folder.svg"
                            else if (model.type === "URL")
                                return "qrc:/assets/images/link.svg"
                            else
                                return ""
                        }
                        anchors.fill: parent
                        anchors.margins: 3
                        sourceSize.width: width
                        sourceSize.height: height
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
                        MenuItem {
                            text: {
                                if (model.type === "URL")
                                    return qsTr("Copy URL")
                                else if (model.type === "CONTAINER")
                                    return qsTr("Copy Title")
                            }
                            onClicked: {
                                if (model.type === "URL")
                                    listModel.copyToClipboard(model.url)
                                else if (model.type === "CONTAINER")
                                    listModel.copyToClipboard(model.title)
                            }
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
            property var _parent: parent
            contentHeight: cld.height //children[0].children[0].children[0].height
            contentWidth: _parent ? _parent.width : 0
            ColumnLayout {
                id: cld
                width: _parent ? _parent.width : 0
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
                    Layout.preferredWidth: parent.width - (2 * Layout.margins)
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WrapAnywhere
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
            property var _parent: parent
            contentHeight: cld.height //children[0].children[0].children[0].height
            contentWidth: _parent ? _parent.width : 0
            ColumnLayout {
                id: cld
                width: _parent ? _parent.width : 0
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
                                "container": listModel.getCurrentContainer(),
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
                    Layout.preferredWidth: parent.width - (2 * Layout.margins)
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WrapAnywhere
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

    Component {
        id: componentImportBookmarks
        ScrollView {
            property var _parent: parent
            contentHeight: cld.height //children[0].children[0].children[0].height
            contentWidth: _parent ? _parent.width : 0
            ColumnLayout {
                id: cld
                width: _parent ? _parent.width : 0
                RowLayout {
                    width: parent.width
                    Button {
                        id: importOk
                        text: qsTr("Import")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: {
                            importInfo.text = ""
                            let t = importViewType.currentValue
                            let f = importFile.text
                            if (t.length <= 0 || f.length <= 0)
                            {
                                importInfo.text = "Please Select Type and File"
                                return
                            }
                            let prevData = listModel.getTypesCount()
                            let res = listModel.importFrom(t, f)
                            if (res) {
                                let currData = listModel.getTypesCount()
                                let txt = "IMPORTED\nPREVIOUS\n"
                                for (let k1 in prevData)
                                    txt += k1 + ": " + prevData[k1] + " "
                                txt += "\nCURRENT\n"
                                for (let k2 in currData)
                                    txt += k2 + ": " + currData[k2] + " "
                                txt += "\nDIFFERENCES\n"
                                for (let k3 in currData)
                                    txt += k3 + ": " + (currData[k3] - prevData[k3]) + " "
                                importInfo.text = txt
                                listModel.goRefresh()
                                //stack.pop()
                            } else
                                importInfo.text = "Error Importing"
                        }
                    }
                    Button {
                        id: importCancel
                        text: qsTr("Close")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: stack.pop()
                    }
                }
                Text {
                    id: importInfo
                    width: parent.width
                    text: ""
                    color: "#FFFFFF"
                    font.bold: true
                    Layout.margins: 5
                    Layout.preferredWidth: parent.width - (2 * Layout.margins)
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WrapAnywhere
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
                        id: importViewType
                        currentIndex: 0
                        textRole: "text"
                        valueRole: "value"
                        model: ListModel {
                            ListElement {text: "Firefox"; value: "FIREFOX_SQLITE"}
                            ListElement {text: "MM Bookmarks"; value: "MMBOOKMARKS"}
                        }
                        Layout.fillWidth: true
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                    Button {
                        text: qsTr("Select File")
                        onClicked: importFileDialog.open()
                        Layout.fillWidth: false
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                }
                RowLayout {
                    width: parent.width
                    Label {
                        text: "File"
                        color: "#B0B0B0"
                        font.italic: true
                        Layout.fillWidth: false
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                    Label {
                        id: importFile
                        color: "#FFFFFF"
                        font.italic: false
                        Layout.fillWidth: true
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WrapAnywhere
                    }
                    FileDialog {
                        id: importFileDialog
                        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
                        onAccepted: importFile.text = listModel.toLocalFile(selectedFile)
                        modality: Qt.ApplicationModal
                        fileMode: FileDialog.OpenFile
                        options: FileDialog.ReadOnly
                        nameFilters: ["SQlite Databases (*.db *.sqlite)", "All Files (*)"]
                    }
                }
            }
        }
    }

    Component {
        id: componentVacuumDatabase
        Dialog {
            title: qsTr("Vacuum Database")
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
                listModel.vacuum()
                destroy()
            }
            onRejected: destroy()
            Text {
                width: parent ? parent.width : 0
                height: parent ? parent.height : 0
                wrapMode: Text.WrapAnywhere
                color: "#FFFFFF"
                text: "Clean database from leftover entries."
            }
        }
    }

    Component {
        id: componentSettings
        ScrollView {
            property var _parent: parent
            property string _cfgDir: ""
            contentHeight: cld.height //children[0].children[0].children[0].height
            contentWidth: _parent ? _parent.width : 0
            ColumnLayout {
                id: cld
                width: _parent ? _parent.width : 0
                RowLayout {
                    width: parent.width
                    Button {
                        id: settingsOk
                        text: qsTr("Save")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        enabled: false
                    }
                    Button {
                        id: settingsCancel
                        text: qsTr("Close")
                        Layout.fillWidth: true
                        Layout.margins: 5
                        onClicked: stack.pop()
                    }
                }
                RowLayout {
                    width: parent.width
                    Label {
                        text: "Database"
                        color: "#B0B0B0"
                        font.italic: true
                        Layout.fillWidth: false
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                    }
                    Label {
                        id: settingsDbDir
                        text: listModel.getDatabasePath()
                        color: "#FFFFFF"
                        font.italic: false
                        Layout.fillWidth: true
                        Layout.margins: 0
                        Layout.leftMargin: 2
                        Layout.rightMargin: 2
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WrapAnywhere
                    }
                }
            }
        }
    }

    FolderDialog {
        id: exportFolderDialog
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            let path = listModel.toLocalFile(selectedFolder)
            let res = listModel.exportTo(path)
            if (res) {
                console.log("Export Succeded")
                listModel.goRefresh()
            } else
                console.log("Export Failed")
        }
        modality: Qt.ApplicationModal
        options: FolderDialog.ShowDirsOnly
    }
}
