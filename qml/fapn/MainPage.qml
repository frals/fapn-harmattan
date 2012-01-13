import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    tools: commonTools

    Rectangle {
	id: headerBox
	anchors.top: parent.top
	anchors.left: parent.left
	anchors.right: parent.right

	height: 72
	color: "#336699"

	Text {
	    id: titleLab
	    anchors.verticalCenter: parent.verticalCenter
	    anchors.left: parent.left
	    anchors.leftMargin: 16
	    text: "Pick or create APN"
	    color: "white"
	    font.pointSize: 24
	}

    }


    ListView {
        id: listView
        clip: true

	anchors { top: headerBox.bottom;
            left: parent.left; right: parent.right;
            margins: 24 }
        height: 48*6

        model: apnhelper.apnList
        delegate: Item {
            width: parent.width - 12
            height: 48

            Rectangle {
                anchors.fill: parent
                color: "#666666"
                visible: mA.pressed
            }

            Text {
                anchors.centerIn: parent
                width: parent.width
                text: modelData
                font.pixelSize: 32
                color: "#808080"
                font.family: "Nokia Pure Text Light"
                elide: Text.ElideRight
            }
            MouseArea {
                id: mA
                anchors.fill: parent
                onClicked: {
                    confirmDialog.apName = modelData
                    confirmDialog.prettyName = apnhelper.apnPrettyName(modelData);
                    confirmDialog.apn = apnhelper.apnAccessPointName(modelData);
                    confirmDialog.open();
                }
            }
        }

	ScrollDecorator {
	    flickableItem: listView
	}

    }

    Column {
        id: layoutCol
        anchors.top: listView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 24
        width: parent.width - 48

        TextField {
            width: parent.width
            id: apname
            placeholderText: "Display name of the access point"
        }

        TextField {
            width: parent.width
            id: iap
            placeholderText: "Connection point (APN)"
        }
    }


//    Button {
//	anchors {
//	    horizontalCenter: parent.horizontalCenter
//	    top: layoutCol.bottom
//	    topMargin: 24
//	}
//	text: "Create!"
//	onClicked: {
//	    if(apname.text != "" && iap.text != "") {
//		apnhelper.createAPN(apname.text, iap.text);
//		goodDialog.open();
//		apname.text = "";
//		iap.text = "";
//	    } else {
//		failDialog.open();
//	    }
//	}
//    }


    ToolBarLayout {
	id: commonTools
	visible: true

	ToolButton {
	    anchors.horizontalCenter: parent.horizontalCenter
	    text: "Create"
	    onClicked: {
		if(apname.text != "" && iap.text != "") {
		    apnhelper.createAPN(apname.text, iap.text);
		    goodDialog.open();
		    apname.text = "";
		    iap.text = "";
		} else {
		    failDialog.open();
		}
	    }
	}

	ToolIcon {
	    platformIconId: "toolbar-view-menu"
	    anchors.right: (parent === undefined) ? undefined : parent.right
	    onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
	}
    }

    Menu {
	id: myMenu
	visualParent: pageStack
	MenuLayout {
	    MenuItem { text: qsTr("About"); onClicked: aboutDialog.open() }
	}
    }

    QueryDialog {
	id: aboutDialog
	titleText: "About fAPN v1.0.0"
	message: "Created by Nick Leppänen Larsson <frals@frals.se>, enjoy!"
	acceptButtonText: "Awesome."
    }

    QueryDialog {
	id: confirmDialog
	property string apName: ""
	property string prettyName: ""
	property string apn: ""
	titleText: "Are you sure?"
	message: "Are you sure you want to remove the AP: " + apName +
		 "\n\nDetails\n" + "Pretty name: " + prettyName + "\n";// +

	acceptButtonText: "Yes"
	rejectButtonText: "No"
	onAccepted: {
	    apnhelper.removeAPN(apName);
	}
    }

    QueryDialog {
	id: failDialog
	titleText: "FAIL!"
	message: "Seems you didn't enter all the data I want. Try again."
	acceptButtonText: ":("
    }

    QueryDialog {
	id: goodDialog
	titleText: "WELL DONE!"
	message: "Your access point should be created, please use the Settings application to configure it fully."
	acceptButtonText: "OK!"
    }

}
