import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtMultimedia 5.5
import MyVideoFilterLib 1.0

ColumnLayout{
// This column layout is not working. It is here just to join videofilter, camera and videoOutput

    Camera {
		//	This is similar with QCamera
        id: camera
        objectName: "Maincamera"
        imageCapture
        {
            onImageCaptured:
            {
                photo.source = preview
            }
        }
        function onCaptureRequired()
        {
		//	If this address must be changed - declare a property to QCamera, and a slot to change it.
           imageCapture.captureToLocation("/storage/emulated/0/DCIM/Camera")
        }

    }
    MyVideoFilter {
	// This is custom class from MyVideoFilter. it is used to actually show pictures
        id: videoFilter

        orientation: videoOutput.orientation
    }
    Image
    {
		//	This may be usefull to save image. I didnt checked this. May be deleted
        visible: false
        id: photo
    }

        VideoOutput {
			//	Video surface wich is showing current frame stream from camera
            id: videoOutput
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: true
            autoOrientation: true
            source: camera
            filters: [ videoFilter ]
         }


}
