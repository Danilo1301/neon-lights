### Teclas:
<p>CTRL + R: Reinicia a config</p>
<p>CTRL + N: Liga/desliga as luzes</p>
<p>CTRL + SHIFT + R: Debug</p>

<br>

### Para adicionar dummy no veículo (opcional)
* ZModeler
* Blender 2.81 (com addon do DragonFF)

<br>

Vídeo com blender: https://www.youtube.com/watch?v=nFGJpmpwkhY

<br>
<br>

### Exemplo de lightGroup: (grupo de luzes)

```json
  "lightGroup_random_name": {
    "vehicle": 579,
    "dummies": [],
    "amount": 40,
    "pattern": "pattern_3",
    "offsetBy": 100,
    "size": 0.08,
    "lerpColor": false,
    "farClip": 600.0,
    "nearClip": 0.3,
    "clone_to": []
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

#### amount
<p>Quantidade de luzes que a linha terá</p>

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
<p>Lista de dummies ou posições em que o groupo de luzes será clonado</p>
<p>Exemplos:</p>

```JavaScript
    "clone_to": [  "d_1"  ], //Clona o lightGroup para onde o dummy d_1 está localizado
    
    "clone_to": [  [1, 0, 0]  ], //Clona o lightGroup para a posição (1, 0, 0)
```
