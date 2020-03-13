import QtQuick 2.5
import QtQuick.Controls 2.12
import QtMultimedia 5.5
import MyVideoFilterLib 1.0

Frame {
    visible: true
    width: 640
    height: 480
    Camera {
        id: camera
    }
    VideoOutput {
        id: videoOutput

        anchors.fill: parent

        autoOrientation: true
        source: camera
        filters: [ videoFilter ]
    }
    MyVideoFilter {
        id: videoFilter

        orientation: videoOutput.orientation
    }

}
