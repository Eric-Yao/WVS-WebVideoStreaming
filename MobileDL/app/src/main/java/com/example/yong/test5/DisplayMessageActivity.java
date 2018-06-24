package com.example.yong.test5;

import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import org.deeplearning4j.nn.conf.NeuralNetConfiguration;
import org.deeplearning4j.nn.conf.layers.DenseLayer;
import org.deeplearning4j.nn.conf.layers.OutputLayer;
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.dataset.DataSet;
import org.nd4j.linalg.factory.Nd4j;

public class DisplayMessageActivity extends AppCompatActivity {
    public int g_interations;
    public float g_learningRate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_message);

        // Get the Intent that started this activity and extract the string
        Intent intent = getIntent();
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);

        // Capture the layout's TextView and set the string as its text
        TextView textView = findViewById(R.id.textView);
        textView.setText(message);

        parser(message);
    }

    private void parser(String message) {
        String wordStart = "(";
        String wordEnd = ")";
        String value;
        int posStart, posEnd;
        //int interations;
        //float learningRate;

        posStart = message.indexOf(wordStart);
        posEnd = message.indexOf(wordEnd);
        g_interations = Integer.valueOf(message.substring(posStart + 1, posEnd));
        Log.d("myTag", Integer.toString(g_interations));
        message = message.substring(posEnd + 1);

        posStart = message.indexOf(wordStart);
        posEnd = message.indexOf(wordEnd);
        g_learningRate = Float.valueOf(message.substring(posStart + 1, posEnd));
        Log.d("myTag", Float.toString(g_learningRate));

        createDL4JModule();
    }

    private void createDL4JModule() {
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                createAndUseNetwork(g_interations, g_learningRate);
            }

            private void createAndUseNetwork(int interations, float learningRate) {
                DenseLayer inputLayer = new DenseLayer.Builder()
                        .nIn(2)
                        .nOut(3)
                        .name("Input")
                        .build();

                DenseLayer hiddenLayer = new DenseLayer.Builder()
                        .nIn(3)
                        .nOut(2)
                        .name("Hidden")
                        .build();

                OutputLayer outputLayer = new OutputLayer.Builder()
                        .nIn(2)
                        .nOut(1)
                        .name("Output")
                        .build();


                NeuralNetConfiguration.Builder nncBuilder = new NeuralNetConfiguration.Builder();
                nncBuilder.iterations(interations);
                nncBuilder.learningRate(learningRate);

                NeuralNetConfiguration.ListBuilder listBuilder = nncBuilder.list();
                listBuilder.layer(0, inputLayer);
                listBuilder.layer(1, hiddenLayer);
                listBuilder.layer(2, outputLayer);

                listBuilder.backprop(true);

                MultiLayerNetwork myNetwork = new MultiLayerNetwork(listBuilder.build());
                myNetwork.init();

                final int NUM_SAMPLES = 4;

                INDArray trainingInputs = Nd4j.zeros(NUM_SAMPLES, inputLayer.getNIn());
                INDArray trainingOutputs = Nd4j.zeros(NUM_SAMPLES, outputLayer.getNOut());

                // If 0,0 show 0
                trainingInputs.putScalar(new int[]{0,0}, 0);
                trainingInputs.putScalar(new int[]{0,1}, 0);
                trainingOutputs.putScalar(new int[]{0,0}, 0);

// If 0,1 show 1
                trainingInputs.putScalar(new int[]{1,0}, 0);
                trainingInputs.putScalar(new int[]{1,1}, 1);
                trainingOutputs.putScalar(new int[]{1,0}, 1);

// If 1,0 show 1
                trainingInputs.putScalar(new int[]{2,0}, 1);
                trainingInputs.putScalar(new int[]{2,1}, 0);
                trainingOutputs.putScalar(new int[]{2,0}, 1);

// If 1,1 show 0
                trainingInputs.putScalar(new int[]{3,0}, 1);
                trainingInputs.putScalar(new int[]{3,1}, 1);
                trainingOutputs.putScalar(new int[]{3,0}, 0);

                Log.d("myTag", "Training is ongoing... ");

                DataSet myData = new DataSet(trainingInputs, trainingOutputs);
                myNetwork.fit(myData);

                Log.d("myTag", "Training is done.");

                // Create input
                INDArray actualInput = Nd4j.zeros(1,2);
                actualInput.putScalar(new int[]{0,0}, 1);
                actualInput.putScalar(new int[]{0,1}, 1);

                // Generate output
                INDArray actualOutput = myNetwork.output(actualInput);
                Log.d("myNetwork Output", actualOutput.toString());

            }
        });


    }
}
