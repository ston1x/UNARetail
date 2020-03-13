import QtQuick 2.5
import QtQuick.Controls 2.12
import QtMultimedia 5.5
import MyVideoFilterLib 1.0

Frame {
    visible: true
    width: parent
    height: parent
    Camera {
        id: camera
		imageCapture
		{
			onImageCaptured:
			{
			photo.source = preview
			}
		}
		
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
	Image
	{
		visible: false
		id: photo
	}
	Button
	{
		id: makeimg
		icon.source: ":/res/new.png"
	}
}
