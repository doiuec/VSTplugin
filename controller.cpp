// 自作VST用のインクルードファイル
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "controller.h"


// VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {


		// クラスを初期化する
		tresult PLUGIN_API MyVSTController::initialize(FUnknown* context)
		{
			// 継承元クラスの初期化を実施
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
				// 初期化に成功
			{
				// パラメーターを追加
				// パラメータは UTF-16 である必要があるので， STR16 で強制
				// processor.cpp と情報のやり取りをするために PARAM1_TAG を指定する
				// この PARAM1_TAG は数値である必要があるが，分かりづらいので myvst3define.h で
				// 数値を指定している．
				parameters.addParameter(STR16("param1"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM1_TAG);
			}

			// 初期化が成功すればkResultTrueを返す。
			result = kResultTrue;
			return result;
		}


	}
} // namespace SteinbergとVstの終わり
