using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_Main : MonoBehaviour {
    [Header(("Set in Inspector"))]
    public GameObject basketPrefab;  // 篮筐预设体
    public int numBaskets = 3;  // 篮筐数
    public float basketMinY = -14.0f;  // 篮筐最小 y
    public float basketSpacingY = 3.0f;  // 相邻篮筐的 y 间隔

    public float gravityDownTime = 10.0f;  // 重力向下的持续时间
    public float gravityUpTime = 1.5f;  // 重力向上的持续时间

    static List<GameObject> basketList;

    GameObject bgmObject;

    void Start() {
        Application.targetFrameRate = 60;

        basketList = new List<GameObject>();
        for (int i = 0; i < numBaskets; i++) {
            GameObject tBasketGO = Instantiate(basketPrefab) as GameObject;
            Vector3 position = Vector3.zero;
            position.y = basketMinY + (basketSpacingY * i);
            tBasketGO.transform.position = position;
            basketList.Add(tBasketGO);
        }

        bgmObject = GameObject.Find("BGMLogicObject");

        Invoke("ReverseGravityDirection", gravityDownTime);
    }

    void ReverseGravityDirection() {
        Vector3 gravity = Vector3.zero;
        gravity.y = -Physics.gravity.y;
        if (Physics.gravity.y < 0) {  // 当前向下
            Invoke("ReverseGravityDirection", gravityUpTime);
        }
        else {  // 当前向上
            Invoke("ReverseGravityDirection", gravityDownTime);
        }
        Physics.gravity = gravity;
    }

    public static void DestroyAllApples() {
        GameObject[] apples = GameObject.FindGameObjectsWithTag("Apple");
        foreach (GameObject apple in apples) {
            Destroy(apple);
        }
    }

    public static void ConsumeABasket() {
        // 丢失一个篮筐
        int topIndex = basketList.Count - 1;
        GameObject topBasket = basketList[topIndex];
        basketList.RemoveAt(topIndex);
        Destroy(topBasket);

        // 若无篮筐, 则 Game Over
        if (topIndex == 0) {
            /*// 更新 HighScore
            int currentScore = Logic_Score.GetCurrentScore();
            if (currentScore > Logic_Score.GetHighScore()) {
                Logic_Score.UpdateHighScore(currentScore);
            }
            
            // 重新开始游戏
            SceneManager.LoadScene("GameScene");*/

            // 切换到 EndScene
            SceneManager.LoadScene("EndScene");
        }
    }

    void Update() {
        if (Input.GetKeyDown(KeyCode.Escape)) {
            if (Time.timeScale > 0.01) {
                Time.timeScale = 0.0f;
                bgmObject.GetComponent<AudioSource>().Pause();
            }
            else {
                Time.timeScale = 1.0f;
                bgmObject.GetComponent<AudioSource>().UnPause();
            }
        }
    }
}
