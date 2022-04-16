// 自作VST用のインクルードファイル
#include "myvst3fuid.h"
#include "processor.h"
#include "myvst3define.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		MyVSTProcessor::MyVSTProcessor()
		{
			// コントローラーのFUIDを設定する
			setControllerClass(ControllerUID);
		}

		// ===================================================================================
		// クラスを初期化する関数(initialize)
		// 入力，出力バスがいくつあるかを教える
		// ===================================================================================
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


		// ===================================================================================
		// ホストとプラグインのバス数が合致しているかどうかをみる(setBusArrangements())
		// ===================================================================================
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

		// ===================================================================================
		// 音声信号を処理する関数(process())
		// オーディオバスへの音声入力データは構造体 data に入ってる
		// 各パラメータは data.inputParameterChanges にアクセス
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data)
		{

			if (data.inputParameterChanges != NULL) {
				// パラメータに変更が加わった場合...(ヌルポインタ出ない場合)
				// 変更されたパラメータの数を取得する
				int32 ParamChangeCount = data.inputParameterChanges->getParameterCount();

				for (int32 i = 0; i < ParamChangeCount; i++) {
					// キューを使って，変更されたパラメータを走査していく．
					// controller で変更されたパラメータを tag で管理する
					// 返り値はタグと変更情報をもつ
					// 変更情報は，サンプル内で複数もつ場合がある．
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);

					// キューの中に変更がある
					if (queue != NULL) {
						int32 tag = queue->getParameterId(); //(Param1)

						// 変更された回数を取得
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						if (queue->getPoint(valueChangeCount - 1, // 取得する変更情報
							sampleOffset,  // 変更情報のキュー内におけるオフセット
							value // 変更内容を格納
						) == kResultTrue) {
							switch (tag) {
							case PARAM1_TAG:
								volume = value;
								break;
							}
						}
					}
				}

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
		}
	}
}
