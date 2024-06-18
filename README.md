# 두더지 게임

두더지 구멍에서 두더지가 튀어나오면 이를 제때 잡는 일반적인 형식을 기반으로 함. 

우선적으로 Tact Switch는 두더지 구멍 역할을 하는 스위치로 활용하도록 하고,

해당 위치의 스위치를 눌러 두더지를 잡는 방식을 채택함. DotMatrix는 게임 화면을 표시하는 디스플레이의 역할임.

8x8 크기의 디스플레이에 두더지 구멍과 두더지를 표현할 수 있다고 판단함. 

Character LCD는 게임 정보(점수, 남은 시간 등)을 표시하는 디스플레이의 역할을 하도록 계획함. 



기존에 존재하는 두더지 잡기와 차별을 둔 부분은 라운드 선택 시스템과 다양한 변수 요소임. 

---
### 기존 프로젝트와 차별점

게임에서는 한 라운드를 끝내면 다음 라운드로 자동적으로 넘어가는 시스템이었다면, 새롭게 계획한 시스템은 자신이 원하는 라운드를 선택하여 자유롭게 난이도를 변경할 수 있도록 하게하는 것임. 다양한 변수 요소로 아이템이나 폭탄과 같은 요소가 있음. 아이템을 통해 점수를 빠르게 얻거나 폭탄으로 인해 점수를 잃는 형태로 게임에 흥미를 높일 수 있다고 판단함.

---
## 플로우 차트
![KakaoTalk_20240617_154314144](https://github.com/Kairo0628/IoT_programming/assets/130293496/cddefbc9-c5f2-4d8c-bdd1-203cfc9873c2)



---
## 게임 시연
![스크린샷 2024-06-18 135457](https://github.com/Kairo0628/IoT_programming/assets/130293496/091295c7-9d6a-4c43-a263-1aa82d74e0d6)


![스크린샷 2024-06-18 135516](https://github.com/Kairo0628/IoT_programming/assets/130293496/742b1ead-d9c8-491d-92b4-8e56b60a4698)

각각 두더지와 폭탄 두더지의 모습

![스크린샷 2024-06-18 135706](https://github.com/Kairo0628/IoT_programming/assets/130293496/eb561820-5ecd-4193-8bfe-e745c1e7bf04)

DIP 스위치를 통한 난이도 조정

![스크린샷 2024-06-18 135649](https://github.com/Kairo0628/IoT_programming/assets/130293496/0d770d46-de0d-4e22-86d7-8d21da11111c)

31점이 되자 게임이 종료되는 모습

