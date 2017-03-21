import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window
    visible: true
    width: 750
    height: 550
    property string selectedTool: "rect"

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open"
                shortcut: "Ctrl + o"
                onTriggered: {
                    fileDialog.visible = true;
                }
            }
            MenuItem {
                text: "Close"
                shortcut: "Ctrl + q"
                onTriggered: {
                    Qt.quit();
                }
            }
        }

        Menu {
            title: "Select"
            MenuItem {
                text: "Rectangle"
                onTriggered: window.selectedTool = "rect"
            }
            MenuItem {
                text: "Ploygon"
                onTriggered: window.selectedTool = "poly"
            }
        }

        Menu {
            title: "Image"
            MenuItem {
                text: "Split"
                shortcut: "Ctrl + s"
                onTriggered: {
                    img.split(image.source);
                }
            }
            MenuItem {
                text: "ROI"
                shortcut: "Ctrl + r"
                onTriggered: img.rectRegionOfInterest(canvas.lastX, canvas.lastY, canvas.rectWidth, canvas.rectHeight);
            }
            MenuItem {
                text: "Palpebral"
                shortcut: "Ctrl + p"
                onTriggered: img.polyRegionOfInterest(canvas.points)
            }
//            MenuItem {
//                text: "Forniceal portion"
//                shortcut: "Ctrl + f"
//                onTriggered: img.polyRegionOfInterest(canvas.points)
//            }
        }
    }

    MessageDialog {
        id: messageDialog
        title: "Erythema Index"
        visible: false
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        visible: false;
        nameFilters: [ "Image files (*.jpg *.png *.tif)", "All files (*)" ]
        onAccepted: {
            image.source = fileDialog.fileUrl;
            fileDialog.visible = false;
        }
        onRejected: {
            console.log("Canceled")
            fileDialog.visible = false;
        }
    }

    Image {
        id: image
        fillMode: Image.PreserveAspectFit
        Canvas {
            id: canvas
            anchors.fill: parent
            property real lastX
            property real lastY
            property real rectWidth: 0
            property real rectHeight: 0
            property variant points: []

            onPaint: {
                var ctx = getContext('2d');
                if(window.selectedTool === "rect"){
                    ctx.reset();
                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "yellow";
                    ctx.strokeRect(lastX, lastY, rectWidth, rectHeight);
                }
                else {
                    ctx.reset();
                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "yellow";
                    ctx.beginPath();
                    ctx.moveTo(canvas.points[0].x,canvas.points[0].y)
                    for(var i = 1 ; i < canvas.points.length; ++i)
                        ctx.lineTo(canvas.points[i].x, canvas.points[i].y);
                    ctx.closePath();
                    ctx.stroke();
                }
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onPressed: {
                    if(window.selectedTool === "rect"){
                        canvas.lastX = mouse.x
                        canvas.lastY = mouse.y
                        canvas.rectWidth = 0;
                        canvas.rectHeight = 0;
                    }
                    else {
                        canvas.points.push(Qt.point(mouse.x, mouse.y));
                    }
                }
                onPositionChanged: {
                    if(window.selectedTool === "rect") {
                        canvas.rectWidth = mouse.x - canvas.lastX;
                        canvas.rectHeight = mouse.y - canvas.lastY;
                        canvas.requestPaint();
                    }
                }
                onReleased: {
                    if(window.selectedTool === "poly") {
                        canvas.points.push(Qt.point(mouse.x, mouse.y));
                        canvas.requestPaint();
                    }
                }
            }
        }
    }

    Connections {
        target: img
        onEIndexChanged: {
            console.log(img.eIndex);
            messageDialog.text = "erythema index = " + img.eIndex;
            messageDialog.visible = true;
        }
    }
}
