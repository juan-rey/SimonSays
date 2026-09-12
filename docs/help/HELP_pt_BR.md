# Guia de Ajuda do SimonSays

## Visão geral
SimonSays é um assistente de texto para fala (TTS) para Windows, criado por Juan Rey Saura, otimizado para fala rápida, bibliotecas de frases e reprodução de sons em linha. Suporta vários idiomas, vozes personalizáveis e recursos práticos para uma comunicação eficiente.

SimonSays usa o espaço livre à esquerda da barra de tarefas do Windows, oferecendo acesso rápido a uma dezena de categorias de frases de uso frequente (e personalizáveis) com apenas alguns cliques.

## Atalhos
- `F1`: Abrir a Ajuda.
- `F2`: Abrir as Configurações.
- `F3`: Configurações de Olhar / Clique por fixação.
- `F4`: Editar a seleção atual.
- `F5` / `F6`: Mover para a seleção anterior / próxima nas listas.
- `F7`: Adicionar após a seleção atual (contexto categoria/frase).
- `F8`: Excluir a seleção atual.
- `F9`: Importar categorias.
- `F10`: Exportar categorias.
- `F11` / `Ctrl -`: Diminuir o zoom (janela de categorias).
- `F12` / `Ctrl +`: Aumentar o zoom (janela de categorias).
- `Ctrl 0`: Redefinir o zoom.
- Botão `Reproduzir>`: Iniciar a reprodução.

## Início e uso básico
1. Inicie o SimonSays; ele aparece na área de notificação do Windows.
2. Para falar, você pode:
   - Digitar um texto personalizado na caixa de entrada e pressionar o botão `Reproduzir>` (ou `Enter`).
   - Clicar em uma ou mais frases salvas na janela de Categorias.
3. Durante a reprodução, o botão mostra `>` para indicar que a reprodução está ativa. Você pode pressioná-lo novamente para parar na hora.

## Janela de Categorias (frases)
- Abra com o botão `Categorias`. Redimensione a janela ou faça o SimonSays lembrar o tamanho nas Configurações.
- Selecione uma categoria na parte de cima e as frases na parte de baixo.
- Um clique em uma frase a reproduz automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativado; caso contrário, a frase é carregada na caixa de entrada principal sem ser falada.

### Gerenciar categorias
- Renomear: selecione a categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover a seleção: `F5`/`F6` para o item anterior/próximo.
- Adicionar: selecione a categoria → (atalho: `F7`) caixa de diálogo `Adicionar nova categoria` → digite o nome → `OK`.
- Excluir: selecione a categoria → (atalho: `F8`) `Excluir categoria`; confirme a exclusão da categoria e das suas frases.



#### Definir ou alterar o ícone de uma categoria
Cada categoria pode mostrar um emoji ou um arquivo `.ico` personalizado no seu botão. O ícone é indicado na janela Adicionar/Editar usando o prefixo separador `##`:

```
<ícone>##<nome da categoria>
```

- **Emoji**: digite o emoji seguido de `##` e do nome.
  - Exemplo: `🙂##Saudações`
- **Arquivo de ícone** (`.ico`): digite o caminho do arquivo (absoluto ou relativo à pasta do SimonSays) seguido de `##` e do nome.
  - Exemplo: `C:\icons\smile.ico##Saudações`
- **Sem ícone**: digite apenas o nome da categoria, sem o prefixo `##`.
  - Exemplo: `Saudações`

> Observação: apenas arquivos `.ico` são suportados para ícones baseados em arquivo. Qualquer outra extensão é ignorada e nenhum ícone é exibido.

### Gerenciar frases
- Editar: selecione a frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover a seleção: `F5`/`F6` para a frase anterior/próxima.
- Adicionar: selecione a frase → (atalho: `F7`) `Adicionar nova frase` → digite o texto → `OK`.
- Excluir: selecione a frase → (atalho: `F8`) `Excluir frase`; confirme.



#### Definir ou alterar o ícone de uma frase
As frases seguem a mesma convenção de prefixo de ícone das categorias, com o separador opcional `##` antes do texto da frase:

```
<ícone>##<texto da frase>
```

- **Emoji**: `👍##Parece bom!`
- **Arquivo de ícone**: `C:\icons\check.ico##Parece bom!`
- **Sem ícone**: `Parece bom!`

As frases também podem incluir um arquivo de áudio em linha. Ao combinar ícone, texto e arquivo de áudio, o formato completo é:

```
<ícone>##<texto da frase>::<arquivo de áudio>
```

- Exemplo: `🔔##Atenção::notification.wav`

O prefixo `##` e o sufixo `::` são opcionais e independentes; você pode usar qualquer combinação:
- Apenas o prefixo de ícone: `🔔##Atenção`
- Apenas o sufixo de áudio: `Atenção::notification.wav`
- Ambos: `🔔##Atenção::notification.wav`
- Nenhum: `Atenção`

## Configurações (F2)
Abra as Configurações (`F2`) para controlar:
- **Texto padrão**: ativar/desativar `Inserir texto padrão automaticamente` e personalizar a frase padrão.
  - `Mostrar teclado tátil` — abre o teclado virtual na tela quando a caixa de entrada recebe o foco, posicionado perto da janela principal.
- **Idioma da interface**: escolher o idioma da interface (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Portuguese (Brazil), Russian, Valencian).
- **Voz**: selecionar uma voz SAPI instalada de acordo com a sua preferência.
- **Volume/Velocidade**: ajustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouvir a voz selecionada.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` após a seleção.
  - `Lembrar o tamanho da janela de categorias`.
- **Comportamento da reprodução**:
  - `Falar a frase imediatamente ao selecionar` (reprodução automática ao clicar).
  - `Parar reprodução anterior` — para automaticamente qualquer reprodução em andamento antes de iniciar uma nova frase.
  - `Aumentar o volume do SimonSays ao reproduzir` — aumenta o volume geral do sistema enquanto fala e o restaura quando a reprodução termina.
  - `Reduzir temporariamente outros áudios ao reproduzir` — reduz o volume de todos os outros aplicativos enquanto fala e o restaura quando a reprodução termina.
- `OK` salva as alterações; `Cancelar` as descarta.

## Olhar / Clique por fixação (F3)
Ative frases e botões sem usar as mãos, mantendo o olhar (ou o cursor do mouse) sobre eles por um tempo definido. Este recurso é novo e precisa de testes e feedback: conte para nós como ele funciona com o seu rastreador ocular pela opção `Enviar comentários` do menu da área de notificação.

Configurações suportadas:
- **Qualquer software de controle ocular que mova o cursor do mouse** (Irisbond EasyClick, Tobii Dynavox Computer Control, modos de cursor do Controle ocular do Windows, OptiKey, ...): o clique por fixação segue o cursor.
- **Leitura direta do olhar, sem precisar mover o cursor**: Irisbond Hiru (modo HID) e dispositivos Tobii por meio do próprio software instalado — verificado com o Tobii Eye Tracker 4C e o Tobii Dynavox PCEye 5; espera-se que o Tobii Eye Tracker 5 funcione, mas ele ainda não foi testado.

Abra as configurações de Olhar / Clique por fixação (`F3`) para configurar (os controles desta caixa de diálogo são exibidos em inglês):
- **Modo de ativação**: escolha entre `Automatic`, que detecta a presença e o comportamento do dispositivo e decide se ativa o clique por fixação pelo cursor do mouse ou pela detecção do olhar (se um rastreador ocular ou um software relacionado for detectado, o clique por fixação é ativado; se nenhum for detectado, ele permanece desativado), `Mouse cursor` para ativar sempre o clique por fixação pelo cursor, `HID eye tracker` para ativá-lo sempre pela detecção do olhar, ou `Off` para desativá-lo sempre, independentemente dos dispositivos presentes.
- **Tempo de fixação** (`Dwell time`): duração (em milissegundos) necessária para acionar um clique ao olhar para um elemento. Durante a fixação, um indicador de progresso é exibido.
- **Raio de tolerância** (`Tolerance radius`): tolerância do olhar (em pixels) para permitir pequenos movimentos dos olhos sem cancelar o clique por fixação.
- **Tempo de espera** (`Cooldown`): milissegundos após uma ativação durante os quais nenhum outro clique por fixação é acionado (evita cliques repetidos acidentais).
- **Cor de progresso** (`Progress color`): cor do indicador de progresso da fixação (por padrão, a cor de destaque do Windows).
- **Detectar como você usa o rastreador** (`Detect how you use the tracker`): dois botões que ajudam o aplicativo a detectar se você move o cursor com um rastreador ocular ou se simplesmente clica nos botões.
- **Detectado** (`Detected`): mostra o tipo de dispositivo detectado e informações adicionais sobre o software relacionado.
- Um botão `Reset` (no canto inferior esquerdo) redefine todas as configurações de fixação para os valores padrão.
- O botão `OK` salva as alterações; `Cancelar` as descarta.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` a interface principal.
- `Configurações`.
- `Gaze / Dwell-click` (Olhar / Clique por fixação).
- `Sobre` mostra a versão e os créditos.
- `Web` abre a página do projeto (se configurada).
- `Enviar comentários` abre o [formulário de feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador padrão.
- `Sair` fecha o SimonSays.

Você pode mover a janela pop-up da área de notificação e outras caixas de diálogo arrastando a barra de título; redimensione a janela de Categorias arrastando as bordas e ative `Lembrar o tamanho da janela de categorias` nas Configurações para manter o tamanho.

## Misturar fala e sons
- Use o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de arquivos de áudio no texto:
  - Exemplo: `Olá ♫notification.wav♫, por favor aguarde.`
  - O texto fora dos delimitadores é falado; o texto entre delimitadores é tratado como um arquivo de som e reproduzido em linha.
- Formatos de áudio suportados: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use caminhos absolutos ou relativos acessíveis ao processo. Use os delimitadores sempre em pares para que o texto final não seja tratado como áudio.



### Ordem de busca dos arquivos de som e ícone
Quando o nome de um arquivo de som ou ícone não contém uma letra de unidade nem um caminho completo, o SimonSays o procura na seguinte ordem, usando a primeira correspondência encontrada:

1. **Pasta AppData** — `%APPDATA%\SimonSays\` (específica do usuário; coloque aqui os arquivos para mantê-los separados da instalação do aplicativo).
2. **Diretório de trabalho** — o diretório de trabalho atual do processo (só é pesquisado se for diferente da pasta do executável).
3. **Pasta do executável** — a pasta que contém `SimonSays.exe`.

Se o arquivo não for encontrado em nenhum desses locais, o som alternativo embutido é usado.

> **Dica:** recomenda-se colocar os seus arquivos de som e ícone personalizados em `%APPDATA%\SimonSays\`; eles são preservados nas atualizações e são específicos do usuário.

## Importar e exportar categorias
- Exportar: você pode exportar todas as categorias ou apenas a selecionada quando solicitado. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F10`).
- Importar: escolha um arquivo; se a categoria já existir, você será perguntado se deseja sobrescrevê-la. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F9`).

## Solução de problemas
- **Falha na inicialização da voz**: verifique se o SAPI está instalado e se existe uma voz compatível.
- **Os sons não são reproduzidos**: confirme o caminho/a extensão e se os delimitadores `♫` estão em pares.
- **Erros de importação/exportação**: verifique as permissões e o local no disco; tente novamente.
- **Idioma da interface não aplicado**: abra novamente as Configurações e confirme a seleção do idioma; verifique se existem textos traduzidos para o idioma escolhido.

## Sobre
- A caixa de diálogo `Sobre` mostra a versão, a descrição e os direitos autorais.
