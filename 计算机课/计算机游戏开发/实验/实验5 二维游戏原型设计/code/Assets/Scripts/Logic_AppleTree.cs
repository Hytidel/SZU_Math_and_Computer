using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_AppleTree : MonoBehaviour {
    [Header("Set in Inspector")]
    // 苹果树的移动
    public float moveSpeed = 10.0f;  // 苹果树移动速度 (单位 / 秒)
    public float xRange = 20.0f;  // 苹果树移动范围为 ±leftAndRightEdge
    public float probability2SwitchDirection = 0.02f;  // 苹果树转向的概率

    // 苹果树掉落苹果
    public GameObject applePrefab;  // 苹果预设体
    public float delay2DropFirstApple = 1.0f;  // 掉落第一个苹果的时间间隔
    public float timeInterval2DropApples = 0.8f;  // 除第一个苹果外, 掉落苹果的时间间隔
    
    void Start() {
        // InvokeRepeating("DropApple", delay2DropFirstApple, timeInterval2DropApples);
        Invoke("DropApple", delay2DropFirstApple);
    }

    void DropApple() {
        GameObject apple = Instantiate<GameObject>(applePrefab);
        apple.transform.position = transform.position;

        Invoke("DropApple", timeInterval2DropApples);
    }

    void Update() {
        transform.Translate(moveSpeed * Time.deltaTime, 0.0f, 0.0f);

        // 超过边界转向
        float x = transform.position.x;
        if (x < -xRange || x > xRange) {
            moveSpeed = -moveSpeed;
        }
    }

    void FixedUpdate() {
        // 10% 的概率转向
        if (Random.value < probability2SwitchDirection) {
            moveSpeed *= -1;
        }
    }
}
