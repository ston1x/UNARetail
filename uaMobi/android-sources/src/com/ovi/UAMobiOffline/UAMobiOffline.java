package com.ovi.UAMobiOffline;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;

import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.OnScanCompletedListener;
//import com.google.zxing.*;
//import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
//import com.google.zxing.common.HybridBinarizer;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class UAMobiOffline extends QtActivity
{

    public static native void fileSelected( String fileName );
    public static native void cameraResponce( String fileName );
    public static native void BarcodeResponce( String fileName );

    static final int REQUEST_OPEN_IMAGE = 1;
    static final int REQUEST_IMAGE_CAPTURE = 1;

    public String lastCameraFileUri;

    private static UAMobiOffline m_instance;

    public UAMobiOffline()
    {
        m_instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    static void openAnImage()
    {
       m_instance.dispatchTakePictureIntent();
    }

    static void openBarcodeScaner()
    {
       m_instance.dispatchTakeBarcodeIntent();
    }

    @Override
    protected void onActivityResult( int requestCode , int resultCode , Intent data )
    {
        if ( resultCode == RESULT_OK &&  requestCode == REQUEST_IMAGE_CAPTURE)
        {
            MediaScannerConnection.scanFile
            (
              getApplicationContext(),
              new String[]{ lastCameraFileUri },
              null,
              new OnScanCompletedListener()
              {
                 @Override
                 public void onScanCompleted( String path , Uri uri )
                 {
                    fileSelected( path );
                    //fileSelected2( path );
                 }
              }
            );
        }
        else if ( resultCode == RESULT_CANCELED &&  requestCode == REQUEST_IMAGE_CAPTURE)
        {
            cameraResponce( "Camera picture canceled" );
            File file = new File( lastCameraFileUri );
            file.delete();
            MediaScannerConnection.scanFile(
                  getApplicationContext(),
                  new String[]{ lastCameraFileUri },
                  null,
                  new OnScanCompletedListener()
                  {
                     @Override
                     public void onScanCompleted( String path , Uri uri )
                     {
                        fileSelected( "NON" );
                     }
                  }
              );
        }

         if (requestCode == 0)
        {
        if (resultCode == RESULT_OK)
        {
            String contents = data.getStringExtra("SCAN_RESULT");
            cameraResponce( contents );
            fileSelected( "NON" );
            BarcodeResponce( contents );
        } else if (resultCode == RESULT_CANCELED)
        {
            fileSelected( "NON" );
            BarcodeResponce( "" );
        }
        }
            super.onActivityResult( requestCode , resultCode , data );
    }

    private File createImageFile() throws IOException
    {
        String timeStamp = new SimpleDateFormat( "yyMMdd_HHmmss" ).format( new Date() );
        String imageFileName = "UAMobi" + timeStamp + "_";

        File storageDir = Environment.getExternalStoragePublicDirectory( Environment.DIRECTORY_PICTURES );
        File image = File.createTempFile( imageFileName , ".jpg" , storageDir );
        return image;
    }

    private void dispatchTakePictureIntent()
    {
        Intent takePictureIntent = new Intent( MediaStore.ACTION_IMAGE_CAPTURE );
        takePictureIntent.putExtra( "android.intent.extra.quickCapture" , true );

        if ( takePictureIntent.resolveActivity( getPackageManager() ) != null )
        {
            File photoFile = null;
            try
            {
                photoFile = m_instance.createImageFile();
            }
            catch ( IOException ex )
            {

            }

            if ( photoFile != null )
            {
                takePictureIntent.putExtra( MediaStore.EXTRA_OUTPUT , Uri.fromFile( photoFile ) );

                lastCameraFileUri = photoFile.toString();

                startActivityForResult( takePictureIntent , REQUEST_IMAGE_CAPTURE );
            }
        }
    }

    private void dispatchTakeBarcodeIntent()
    {
        Intent intent = new Intent( "com.google.zxing.client.android.SCAN" );
        intent.setPackage( "com.google.zxing.client.android" );
        startActivityForResult(intent, 0);
    }

    public String getRealPathFromURI(Context context, Uri contentUri)
    {
        Cursor cursor = null;
        try
        {
            String[] proj = { MediaStore.Images.Media.DATA };
            cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
        finally
        {
            if (cursor != null)
            {
                cursor.close();
            }
        }
    }

    public static int return_string()
    {
        return 1;
    }
}
