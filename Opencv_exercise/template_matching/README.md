# Template matching
## 使い方
* commandline引数の第二引数に全体画像を、第三引数に全体画像中で探索したい部分画像（テンプレート）を指定します
* 出力はテンプレートにもっとも類似する領域を色反転して表示した全体画像となります

## 結果の例

### 例1 (Lenna)
|入力・全体画像 (Lenna.bmp)|入力・テンプレート (Lenna_tml_1.bmp)|出力 (Lenna_template_matching_1.png)|
|:---:|:---:|:---:|
|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Lenna.bmp)|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Lenna_tml_1.bmp)|![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/results/Lenna_template_matchimg_1.png)|
### 例2 (Mikke)
|入力・全体画像 (mikke_1.jpg)|入力・テンプレート (mikke_tml_1.jpg)|出力 (mikke_template_matchimg_1.png)|
|:---:|:---:|:---:|
|![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/other/mikke_1.jpg)|![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/other/mikke_tml_1.jpg) |![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/results/mikke_template_matchimg_1.png)|

### 例3 (Pepper)
|入力・全体画像 (Pepper.bmp)|入力・テンプレート (Pepper_tml_1.bmp)|出力 (Pepper_template_matching_1.png)|
|:---:|:---:|:---:|
|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Pepper.bmp)|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Pepper_tml_1.bmp) |![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/results/Pepper_template_matchimg_1.png)|


### 失敗例 (Lenna)
|入力・全体画像 (Lenna.bmp)|入力・テンプレート (Lenna_tml_5.bmp)|出力 (Lenna_template_matching_5.png)|
|:---:|:---:|:---:|
|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Lenna.bmp)|![](https://raw.githubusercontent.com/Atsushi-Kobayashi/mic_programming_exercise/master/images/other/Lenna_tml_5.bmp)|![](https://github.com/Atsushi-Kobayashi/mic_programming_exercise/blob/master/images/results/Lenna_template_matchimg_5.png)|
