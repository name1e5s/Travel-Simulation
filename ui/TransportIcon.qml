import QtQuick 2.4
import QtLocation 5.6

MapQuickItem {
    property string transName;
    property int bearing: 0;
    property url transImage;
    visible: true;

    anchorPoint.x: image.width/2
    anchorPoint.y: image.height/2

    sourceItem: Grid {
        columns: 1
        Grid {
            horizontalItemAlignment: Grid.AlignHCenter
            Image {
                id: image
                rotation: bearing
                source: transImage;
            }
            Rectangle {
                id: bubble
                color: "lightblue"
                border.width: 1
                width: text.width * 1.3
                height: text.height * 1.3
                radius: 5
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: transName
                }
            }
        }
    }
}
