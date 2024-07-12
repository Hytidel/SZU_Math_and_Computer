using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_StartButton : MonoBehaviour {
    public void StartButtonClicked() {
        // ¿ªÊ¼ÓÎÏ·
        SceneManager.LoadScene("GameScene");
    }
}
