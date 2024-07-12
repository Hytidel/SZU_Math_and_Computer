using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;
using TMPro;

public class Logic_EndScene : MonoBehaviour {
    static TMP_Text GetScoreText() {
        return GameObject.Find("Score").GetComponent<TMP_Text>();
    }

    void Start() {
        // 显示当前分数
        int currentScore = Logic_Score.GetCurrentScore();
        Debug.Log(currentScore);
        Debug.Log(Logic_Score.GetHighScore());
        TMP_Text score = GetScoreText();
        score.text = currentScore.ToString();

        // 隐藏破纪录图片
        GameObject breakRecord = GameObject.Find("BreakRecord");
        breakRecord.SetActive(false);

        // 若破纪录, 显示破纪录的图片, 并更新 HighScore
        if (currentScore > Logic_Score.GetHighScore()) {
            Debug.Log("Break");
            breakRecord.SetActive(true);
            Logic_Score.UpdateHighScore(currentScore);
        }
    }
}
