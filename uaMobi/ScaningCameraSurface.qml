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
           Rectangle{
               id: non_captureZoneTop
               color: "blue"
               opacity: 0.2
               width: parent.width
               height: parent.height /4
               anchors.top: videoOutput.top
           }
           Rectangle
           {
                id: non_captureZoneBot
                color: "blue"
                opacity: 0.2
                width: parent.width
                height: parent.height/4
                anchors.bottom: videoOutput.bottom
           }
           Rectangle
           {
               id: non_captureZoneLeft
               color:"blue"
               opacity: 0.2
               width: parent.width/4
               height: parent.height/2
               anchors.left: videoOutput.left
                anchors.bottom: non_captureZoneBot.top
           }
           Rectangle
           {
               id: non_captureZoneRight
               color:"blue"
               opacity: 0.2
               width: parent.width/4
               height: parent.height/2
               anchors.right: videoOutput.right
               anchors.bottom: non_captureZoneBot.top
           }
       }

       QZXingFilter
       {
           objectName: "FilterZX"
           id: zxingFilter
           captureRect: {
               // setup bindings
               videoOutput.contentRect;
               videoOutput.sourceRect;
               return videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(
                   0.25, 0.25, 0.5, 0.5
               )));
           }
           signal bcArr(string str)
           function emitbc(bc)
           {
               bcArr(bc)
           }
            decoder {
               objectName: "DecoderZX"
               enabledDecoders: QZXing.DecoderFormat_CODE_128
                                | QZXing.DecoderFormat_CODE_128_GS1 | QZXing.DecoderFormat_CODE_39
                                | QZXing.DecoderFormat_UPC_EAN_EXTENSION | QZXing.DecoderFormat_EAN_8 | QZXing.DecoderFormat_EAN_13
               | QZXing.DecoderFormat_PDF_417;

               onTagFound: {
                   console.log(tag + " | " + decoder.foundedFormat() + " | " + decoder.charSet());
                    zxingFilter.emitbc(tag);
                }
               tryHarder: false
           }

           onDecodingStarted:
           {
               console.log("started");
           }
            onBcArr:
            {

            }

           property int framesDecoded: 0
           property real timePerFrameDecode: 0

           onDecodingFinished:
           {
              timePerFrameDecode = (decodeTime + framesDecoded * timePerFrameDecode) / (framesDecoded + 1);
              framesDecoded++;
              console.log("frame finished: " + succeeded, decodeTime, timePerFrameDecode, framesDecoded);
           }
       }




}
