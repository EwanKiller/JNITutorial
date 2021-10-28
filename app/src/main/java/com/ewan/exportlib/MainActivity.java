package com.ewan.exportlib;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.TextView;

import com.ewan.exportlib.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static final String TAG = "Ewan";

    static final int[] intArray = { 1, 2, 3 };

//    static final float[] rotation = {1.0f, 2.0f, 3.0f, 4.0f};

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

        /**
         * 1. Java call native c++
         * 没有返回值，无参数
         */
        initialize();

        /**
         * 2. Java call native c++
         * 没有返回值，参数为int数组
         */
        passIntArray(intArray);

        /**
         * 3. Java call native c++
         * 返回值为float数组，无参数
         */
        float[] returnFloatArray = returnFloatArray();
        Log.d(TAG,"return float array :" + returnFloatArray[0] + "," + returnFloatArray[1] + "," + returnFloatArray[2]);

        /**
         * 调native c++ 让cpp里的函数可以被调用
         */
        callNative();
    }

    /**
     * A native method that is implemented by the 'exportlib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 1.声明native method : 没有返回值，无参数
     * */
    public native void initialize();

    /**
     * 2.声明native method : 没有返回值，参数为 int数组
     * @param intArray
     */
    public native void passIntArray(int[] intArray);

    /**
     * 3.声明native method : 返回值为float数组，无参数
     * @return
     */
    public native float[] returnFloatArray();

    /**
     * 调一个native c++来执行c++里的函数
     */
    public native void callNative();

    /**
     * 4.called by c++ : 无返回值，无参数
     */
    public void Initialize() {
        Log.d(TAG, "native c++ call Java successful");
    }

    /**
     * 5. called by c++ : 无返回值，参数为string
     * @param funcName
     */
    public void PassStringValue(String funcName) {
        Log.d(TAG, funcName);
    }

    /**
     * 6. called bt c++ : 返回值为string， 无参数
     * @return
     */
    public String ReturnString() {
        return "ReturnString";
    }
}