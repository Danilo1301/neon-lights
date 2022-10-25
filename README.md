### Versão com menu:
https://github.com/Danilo1301/neon-lights/tree/v2

<br>

### Teclas:
<p>CTRL + R: Reinicia a config</p>
<p>CTRL + N: Liga/desliga as luzes</p>
<p>CTRL + SHIFT + D: Debug</p>

<br>

### Para adicionar dummy no veículo (opcional)
* ZModeler
* Blender (com addon do [DragonFF](https://gtaforums.com/topic/925364-dragonff-blender-gta-io))

<br>

Vídeo com blender: https://www.youtube.com/watch?v=nFGJpmpwkhY

<br>
<br>

### Exemplo de lightGroup: (grupo de luzes)

```js
  "lightGroup_random_name": {
    "vehicle": 579,
    "dummies": [],
    "lights_distance": 0.1,
    "pattern": "pattern_3",
    "offsetBy": 100,
    "size": 0.08,
    "lerpColor": false,
    "farClip": 600.0,
    "nearClip": 0.3,
    
    "clone_to": [],    //opcional
    "fixPatternOffset": false    //opcional
  }
```

#### vehicle
<p>ID do veículo</p>
<a href="https://sampwiki.blast.hk/wiki/Vehicles:All">https://sampwiki.blast.hk/wiki/Vehicles:All</a>

<br>
<br>

#### dummies
<p>Lista de dummies ou posições</p>
<p>Será formado uma linha entre cada dummy dentro da lista</p>
<p>Exemplos:</p>

```JavaScript
    "dummies": [  "d_1",  "d_2",  "d_3"  ], //Cria uma linha começando no dummy d_1, passando pelo d_2 e terminando no d_3
    
    "dummies": [  [1, 0, 0],  [-1, 0, 0]  ], //Cria uma linha começando na esquerda (x= 1), terminando na direita (x= -1)
    
    "dummies": [  "d_1",  [-1, 0, 0]  ], //Cria uma linha começando no dummy d_1, terminando na direita (x= -1)
```

<p>Separar:</p>
<p>Você pode separar uma lista de dummies usando "-"</p>

```JavaScript
    "dummies": [  "d_1",  "d_2", "-", "d_3", "d_4"  ], //Cria uma primeira linha começando no dummy d_1 e terminando no d_2, e cria uma segunda linha começando no dummy d_3 e terminando no d_4
```

<br>

#### lights_distance
<p>Distância entre cada luz</p>

<br>

#### pattern
<p>Padrão em que as luzes piscarão</p>
<p>Os padrões são definidos na parte "patterns" (ex: pattern_1, pattern_2, rainbow)</p>

<br>

#### offsetBy
<p>Configurar a velocidade em que as luzes piscam</p>

<br>

#### size
<p>Tamanho das luzes</p>

<br>

#### lerpColor
<p>Transição entre cor anterior e cor atual</p>
<p>true - Ativado</p>
<p>false - Desativado</p>

<br>

#### farClip
<p>Distância em que as luzes ficarão visíveis</p>

<br>

#### nearClip
<p>Distância em que as luzes começam a atravessar por dentro de outros objetos muito próximos</p>

<br>


#### clone_to
<p>Clone o grupo de luzes atual em outro dummy ou em uma posição, podendo espelhar os pontos (no eixo X e Y), e também inverter o padrão</p>
<p>Exemplos:</p>

```JavaScript
    "clone_to":
     [
       { "dummy": "d_1" }   //Clona o lightGroup para onde o dummy d_1 está localizado
     ],
    
    "clone_to":
    [
      { "dummy": [2, 0, 0] }   //Clona o lightGroup para a posição (2, 0, 0)
    ],
```

Opções para o clone_to:

```JavaScript
  {
    "dummy": "dummyName",
    
     "flipX": false,    //opcional
     "flipY": false,    //opcional
     "invertOffset": false,    //opcional

     "offsetX": 0.0, //opcional
     "offsetY": 0.0, //opcional
     "offsetZ": 0.0 //opcional
  }
```