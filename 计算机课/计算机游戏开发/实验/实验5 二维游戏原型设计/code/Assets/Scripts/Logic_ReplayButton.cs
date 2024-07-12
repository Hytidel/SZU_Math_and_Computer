using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_ReplayButton : MonoBehaviour {
    public void ReplayButtonClicked() {
        // 重新开始游戏
        SceneManager.LoadScene("GameScene");
    }
}
