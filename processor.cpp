// 自作VST用のインクルードファイル
#include "myvst3fuid.h"
#include "processor.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg{
namespace Vst {


// クラスを初期化する関数(initialize)
// 入力，出力バスがいくつあるかを教える
tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context)
{
	// まず継承元クラスの初期化を実施
	tresult result = AudioEffect::initialize(context);
	if (result == kResultTrue)
	{
		// 入力と出力を設定
		addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
		addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
		// kStereo::ステレオ設定
		// モノラルは kMono

		// イベント入力・出力
		// addEventInput();
		// addEventOutput();

		// 今回は何もしない
	}


	// 初期化が成功すればkResultTrueを返す。
	return result;
}


// ホストとプラグインのバス数が合致しているかどうかをみる(setBusArrangements())
tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
{
	// inputとoutputのバスが1つずつで、かつinputとoutputの構成がステレオの場合
	if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}

	// inputとoutputのバスが1つずつで、かつinputがモノラル，outputの構成がステレオの場合
	else if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kMono && outputs[0] == SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}

	// 対応していないバス構成の場合、kResultFalseを返す。
	return kResultFalse;
}


// 音声信号を処理する関数(process())
// オーディオバスへの音声入力データは構造体 data に入ってる
tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data)
{
	if (data.numInputs == 1 && data.numOutputs == 1) {
		// data.numInputs で，オーディオバス数を取得する．
		// プラグインで用意したバス数ではなく，ホストで用意したバス数
		if (data.inputs[0].numChannels != 2 || data.outputs[0].numChannels != 2) {
			// 入出力がステレオ
			// channelBuffer32[バスチャンネル数]
			// ステレオなので，チャンネルはふたつあり，[0], [1]
			// 各バッファのポインタを変数に格納
			Sample32* inL = data.inputs[0].channelBuffers32[0];
			Sample32* inR = data.inputs[0].channelBuffers32[1];
			Sample32* outL = data.inputs[0].channelBuffers32[0];
			Sample32* outR = data.inputs[0].channelBuffers32[1];

			// numSamples だけプロセッサ処理(numSamples は，音声のデータサイズ)
			// 今回は，入力出力をコピーするだけ
			for (int32 i = 0; i < data.numSamples; i++) {
				outL[i] = inL[i];
				outR[i] = inR[i];
			}
			return kResultTrue;
		}

		if (data.inputs[0].numChannels != 1 || data.outputs[0].numChannels != 2) {
			// 入力がモノラル出力がステレオ
			Sample32* in = data.inputs[0].channelBuffers32[0];
			Sample32* outL = data.inputs[0].channelBuffers32[0];
			Sample32* outR = data.inputs[0].channelBuffers32[1];

			// numSamples だけプロセッサ処理(numSamples は，音声のデータサイズ)
			// 今回は，入力出力をコピーするだけ
			for (int32 i = 0; i < data.numSamples; i++) {
				outL[i] = in[i];
				outR[i] = in[i];
			}
			return kResultTrue;
		}
	}
}
