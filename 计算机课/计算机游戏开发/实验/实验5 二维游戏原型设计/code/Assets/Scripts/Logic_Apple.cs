using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_Apple : MonoBehaviour {
    [Header("Set in Inspector")]
    public float yMin = -25.0f;  // ×îÐ¡ y Öµ

    void Update() {
        if (transform.position.y < yMin) {
            Destroy(this.gameObject);

            Logic_Main.DestroyAllApples();
            Logic_Main.ConsumeABasket();
        }
    }
}
