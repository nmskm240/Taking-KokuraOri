using System;
using System.IO;
using Google.Cloud.TextToSpeech.V1;
using System.Diagnostics;

public class QuickStart
{
    public static void Main(string[] args)
    {
        var fileName = "";

        if(args.Length <= 0)
        {
            Console.WriteLine("音声化するテキストを入力してください。");
            fileName = Console.ReadLine();
        }
        else
        {
            fileName = args[0];
        }

        var credentialsFilePath = @"talking-kokuraori-9658a1ec2ebe.json";

        var textToSpeechClientBuilder = new TextToSpeechClientBuilder()
        {
            CredentialsPath = credentialsFilePath
        };
        var client = textToSpeechClientBuilder.Build();

        // 読み上げテキストの設定
        SynthesisInput input = new SynthesisInput
        {
            Text = fileName
        };

        // 音声タイプの設定
        VoiceSelectionParams voice = new VoiceSelectionParams
        {
            Name = "ja-JP-Wavenet-A",
            LanguageCode = "ja-JP",
            SsmlGender = SsmlVoiceGender.Neutral
        };

        // オーディオ出力の設定
        AudioConfig config = new AudioConfig
        {
            AudioEncoding = AudioEncoding.Linear16,
            Pitch = -2.0
        };

        // Text-to-Speech リクエストの生成
        var response = client.SynthesizeSpeech(new SynthesizeSpeechRequest
        {
            Input = input,
            Voice = voice,
            AudioConfig = config
        });

        // Text-to-Speech レスポンス（音声ファイル）の保存
        fileName = "../../../../input/" +  fileName + ".wav";
        using (Stream output = File.Create(fileName))
        {
            response.AudioContent.WriteTo(output);
            Console.WriteLine($"音声コンテンツを '{fileName}' として保存しました。");
        }
    }
}