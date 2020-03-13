import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtMultimedia 5.5
import QZXing 2.3

Rectangle{
    id: aroundrect
    width: 480
    height: 600

    Camera {
        id: camera
        objectName: "Maincamera"
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }
        imageCapture
        {
            onImageCaptured:
            {

                photo.source = preview;
            }
        }
        function onCaptureRequired()
        {
           imageCapture.captureToLocation("/storage/emulated/0/DCIM/Camera");
        }

    }
    VideoOutput
       {
           id: videoOutput
                      source: camera
                      autoOrientation: true
                      fillMode: VideoOutput.Stretch
                      filters: [ zxingFilter ]
                      width: parent.width
                      height: parent.height
       }

       QZXingFilter
       {
           id: zxingFilter
           decoder {
               enabledDecoders: QZXing.DecoderFormat_None
			   
               tryHarder: false
           }
       }
	   		 	  
}
