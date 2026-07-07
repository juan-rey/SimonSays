# Guia de Ajuda do SimonSays

## Visão geral
SimonSays é um assistente de texto para fala (TTS) para Windows, criado por Juan Rey Saura, otimizado para fala rápida, bibliotecas de frases e reprodução de sons em linha. Suporta vários idiomas, vozes personalizáveis e funcionalidades práticas para uma comunicação eficiente.

SimonSays utiliza o espaço livre à esquerda da barra de tarefas do Windows, proporcionando acesso rápido a uma dezena de categorias de frases de uso frequente (e personalizáveis) com apenas alguns cliques.

## Atalhos
- `F1`: Abrir Ajuda.
- `F2`: Abrir Definições.
- `F3`: Definições de Olhar / Clique por fixação.
- `F4`: Editar a seleção atual.
- `F5` / `F6`: Mover para a seleção anterior / seguinte nas listas.
- `F7`: Adicionar após a seleção atual (contexto categoria/frase).
- `F8`: Eliminar a seleção atual.
- `F9`: Importar categorias.
- `F10`: Exportar categorias.
- `F11` / `Ctrl -`: Reduzir zoom (janela de categorias).
- `F12` / `Ctrl +`: Aumentar zoom (janela de categorias).
- `Ctrl 0`: Repor zoom.
- Botão `Reproduzir>`: Iniciar reprodução.

## Início e utilização básica
1. Inicie o SimonSays; aparece na área de notificação do Windows.
2. Para falar, pode:
   - Escrever texto personalizado na caixa de entrada e premir o botão `Reproduzir>` (ou `Enter`).
   - Clicar numa ou mais frases guardadas na janela de Categorias.
3. Durante a reprodução, o botão mostra `>` para indicar reprodução ativa. Pode premir novamente para parar instantaneamente.

## Janela de Categorias (frases)
- Abra com o botão `Categorias`. Redimensione ou recorde o tamanho através das Definições.
- Selecione uma categoria em cima e frases em baixo.
- Um clique numa frase reproduz automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativado; caso contrário, carrega a frase na caixa de entrada principal sem falar.

### Gerir categorias
- Renomear: selecione categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover seleção: `F5`/`F6` para o item anterior/seguinte.
- Adicionar: selecione categoria → (atalho: `F7`) diálogo `Adicionar nova categoria` → introduza o nome → `OK`.
- Eliminar: selecione categoria → (atalho: `F8`) `Eliminar categoria`; confirme a eliminação da categoria e das suas frases.



#### Definir ou alterar o ícone de uma categoria
Cada categoria pode mostrar um emoji ou um ficheiro `.ico` personalizado no respetivo botão. O ícone é indicado na janela Adicionar/Editar usando o prefixo separador `##`:

```
<ícone>##<nome da categoria>
```

- **Emoji**: introduza o emoji seguido de `##` e do nome.
  - Exemplo: `🙂##Saudações`
- **Ficheiro de ícone** (`.ico`): introduza o caminho do ficheiro (absoluto ou relativo à pasta do SimonSays) seguido de `##` e do nome.
  - Exemplo: `C:\icons\smile.ico##Saudações`
- **Sem ícone**: introduza apenas o nome da categoria, sem prefixo `##`.
  - Exemplo: `Saudações`

> Nota: apenas ficheiros `.ico` são suportados para ícones baseados em ficheiro. Qualquer outra extensão é ignorada e nenhum ícone é apresentado.

### Gerir frases
- Editar: selecione frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover seleção: `F5`/`F6` para a frase anterior/seguinte.
- Adicionar: selecione frase → (atalho: `F7`) `Adicionar nova frase` → introduza o texto → `OK`.
- Eliminar: selecione frase → (atalho: `F8`) `Eliminar frase`; confirme.



#### Definir ou alterar o ícone de uma frase
As frases seguem a mesma convenção de prefixo de ícone das categorias, com o separador opcional `##` antes do texto da frase:

```
<ícone>##<texto da frase>
```

- **Emoji**: `👍##Parece bem!`
- **Ficheiro de ícone**: `C:\icons\check.ico##Parece bem!`
- **Sem ícone**: `Parece bem!`

As frases também podem incluir um ficheiro de áudio em linha. Ao combinar ícone, texto e ficheiro de áudio, o formato completo é:

```
<ícone>##<texto da frase>::<ficheiro de áudio>
```

- Exemplo: `🔔##Atenção::notification.wav`

O prefixo `##` e o sufixo `::` são opcionais e independentes; pode usar qualquer combinação:
- Apenas prefixo de ícone: `🔔##Atenção`
- Apenas sufixo de áudio: `Atenção::notification.wav`
- Ambos: `🔔##Atenção::notification.wav`
- Nenhum: `Atenção`

## Definições (F2)
Abra as Definições (`F2`) para controlar:
- **Texto predefinido**: ativar/desativar `Inserir texto predefinido automaticamente` e personalizar a frase predefinida.
  - `Mostrar teclado tátil` — abre o teclado tátil no ecrã quando a caixa de entrada recebe o foco, posicionado perto da janela principal.
- **Idioma da interface**: escolher idioma da UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: selecionar voz SAPI instalada de acordo com a sua preferência.
- **Volume/Velocidade**: ajustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouvir a voz selecionada.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` após seleção.
  - `Lembrar o tamanho da janela de categorias`.
- **Comportamento de reprodução**:
  - `Falar a frase imediatamente ao selecionar` (reprodução automática ao clicar).
  - `Parar reprodução anterior` — para automaticamente qualquer reprodução em curso antes de iniciar uma nova frase.
  - `Aumentar o volume do SimonSays ao reproduzir` — aumenta o volume mestre do sistema enquanto fala, restaurando-o quando a reprodução termina.
  - `Reduzir temporariamente outros áudios ao reproduzir` — reduz o volume de todas as outras aplicações enquanto fala, restaurando-o quando a reprodução termina.
- `OK` guarda as alterações; `Cancelar` descarta.

## Olhar / Clique por fixação (F3)
Ative frases e botões sem usar as mãos, mantendo o olhar (ou o cursor do rato) sobre eles durante um tempo definido. Esta funcionalidade é nova — precisa de testes e feedback: conte-nos como funciona com o seu dispositivo de seguimento ocular através da opção `Comentários` do menu da área de notificação.

Configurações suportadas:
- **Qualquer software de controlo ocular que mova o cursor do rato** (Irisbond EasyClick, Tobii Dynavox Computer Control, modos de cursor do Controlo ocular do Windows, OptiKey, ...): o clique por fixação segue o cursor.
- **Leitura direta do olhar, sem necessidade de mover o cursor**: Irisbond Hiru (modo HID) e dispositivos Tobii através do seu próprio software instalado — verificado com o Tobii Eye Tracker 4C e o Tobii Dynavox PCEye 5; espera-se que o Tobii Eye Tracker 5 funcione, mas ainda não foi testado.

Abra as definições de Olhar / Clique por fixação (`F3`) para configurar (os controlos deste diálogo são apresentados em inglês):
- **Modo de ativação**: escolha entre `Automatic`, que deteta a presença e o comportamento do dispositivo e decide se ativa o clique por fixação através do cursor do rato ou através da deteção do olhar (se for detetado um dispositivo de seguimento ocular ou software relacionado, o clique por fixação é ativado; se nenhum for detetado, permanece desativado), `Mouse cursor` para ativar sempre o clique por fixação através do cursor, `HID eye tracker` para o ativar sempre através da deteção do olhar, ou `Off` para o desativar sempre, independentemente dos dispositivos presentes.
- **Tempo de fixação** (`Dwell time`): duração (em milissegundos) necessária para acionar um clique ao olhar para um elemento. Durante a fixação é mostrado um indicador de progresso.
- **Raio de tolerância** (`Tolerance radius`): tolerância do olhar (em píxeis) para permitir pequenos movimentos dos olhos sem cancelar o clique por fixação.
- **Tempo de espera** (`Cooldown`): milissegundos após uma ativação durante os quais não será acionado outro clique por fixação (evita cliques repetidos acidentais).
- **Cor de progresso** (`Progress color`): cor do indicador de progresso da fixação (por predefinição, a cor de destaque do Windows).
- **Detetar como usa o dispositivo** (`Detect how you use the tracker`): dois botões que ajudam a aplicação a detetar se move o cursor com um dispositivo de seguimento ocular ou se simplesmente clica nos botões.
- **Detetado** (`Detected`): mostra o tipo de dispositivo detetado e informações adicionais sobre o software relacionado.
- Um botão `Reset` (em baixo à esquerda) repõe todas as definições de fixação nos valores predefinidos.
- O botão `OK` guarda as alterações; `Cancelar` descarta-as.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` interface principal.
- `Definições`.
- `Gaze / Dwell-click` (Olhar / Clique por fixação).
- `Sobre` mostra versão e créditos.
- `Web` abre a página do projeto (se configurada).
- `Comentários` abre o [formulário de feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador padrão.
- `Sair` fecha o SimonSays.

Pode mover a janela pop-up da área de notificação e outros diálogos arrastando a barra de título; redimensione a janela de Categorias arrastando as bordas, e ative `Lembrar o tamanho da janela de categorias` nas Definições para manter o tamanho.

## Misturar fala e sons
- Use o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de ficheiros de áudio no texto:
  - Exemplo: `Olá ♫notification.wav♫, por favor aguarde.`
  - O texto fora dos delimitadores é falado; o texto entre delimitadores é tratado como ficheiro de som e reproduzido em linha.
- Formatos de áudio suportados: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use caminhos absolutos ou relativos acessíveis ao processo. Emparelhe delimitadores para evitar que o texto final seja tratado como áudio.



### Ordem de procura de ficheiros de som e ícone
Quando um nome de ficheiro de som ou ícone não contém uma letra de unidade ou um caminho completo, o SimonSays procura-o pela seguinte ordem, usando a primeira correspondência encontrada:

1. **Pasta AppData** — `%APPDATA%\SimonSays\` (específica do utilizador; coloque aqui os ficheiros para os manter separados da instalação da aplicação).
2. **Diretório de trabalho** — o diretório de trabalho atual do processo (apenas é pesquisado se for diferente da pasta do executável).
3. **Pasta do executável** — a pasta que contém `SimonSays.exe`.

Se o ficheiro não for encontrado em nenhuma destas localizações, é usado o som de reserva incorporado.

> **Sugestão:** recomenda-se colocar os seus ficheiros de som e ícone personalizados em `%APPDATA%\SimonSays\`; sobreviverão às atualizações e serão específicos do utilizador.

## Importar e exportar categorias
- Exportar: pode exportar todas as categorias ou apenas a selecionada quando solicitado. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F10`).
- Importar: escolha um ficheiro; se a categoria existir, será perguntado se deseja sobrescrever. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F9`).

## Resolução de problemas
- **Falha na inicialização da voz**: verifique se o SAPI está instalado e se existe uma voz compatível.
- **Sem reprodução de sons**: confirme caminho/extensão e delimitadores `♫` emparelhados.
- **Erros de importação/exportação**: verifique permissões e localização do disco; tente novamente.
- **Idioma da UI não aplicado**: reabra as Definições e confirme a seleção de idioma; certifique-se de que existem cadeias de localização para o idioma escolhido.

## Sobre
- O diálogo `Sobre` mostra versão, descrição e direitos de autor.
