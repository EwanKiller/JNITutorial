package com.ewan.exportlib;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.TextView;

import com.ewan.exportlib.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static final String TAG = "Ewan";

    // Used to load the 'exportlib' library on application startup.
    static {
        System.loadLibrary("exportlib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        // Call local method
        float[] rot = new float[4];
        setDeviceRot(rot);
    }

    /**
     * A native method that is implemented by the 'exportlib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * A native method that pass float[] and return float[]
     * @param rot
     * @return
     */
    public native float[] sendDeviceRot(float[] rot);

    public void setDeviceRot(float[] rot) {
        rot[0] = 0.0f;
        rot[1] = 1.0f;
        rot[2] = 2.0f;
        rot[3] = 3.0f;
        float[] back = sendDeviceRot(rot);
        Log.e(TAG, String.valueOf(back[0]));
        Log.e(TAG, String.valueOf(back[1]));
        Log.e(TAG, String.valueOf(back[2]));
        Log.e(TAG, String.valueOf(back[3]));
    }
}