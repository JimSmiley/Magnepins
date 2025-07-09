using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveUpAndDown : MonoBehaviour
{
    // Start is called before the first frame update
    public float moveBy;
    Transform getTransform;
    void Start()
    {
        getTransform = this.gameObject.transform;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown (KeyCode.DownArrow))
        {
            transform.Translate(0, -moveBy, 0); 
        }
        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            transform.Translate(0, moveBy, 0);
        }
    }
}
