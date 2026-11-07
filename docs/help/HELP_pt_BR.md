# Guia de ajuda do SimonSays

## Visão geral
O SimonSays é um assistente de conversão de texto em fala (TTS) para Windows criado por Juan Rey Saura. Ele nasceu como uma ferramenta para pessoas com ELA e outras pessoas que não conseguem mais falar, para que pudessem se comunicar rapidamente sem largar o que estavam fazendo no computador. Com o tempo ficou claro que o mesmo conceito serve como ferramenta geral de CAA (comunicação aumentativa e alternativa) para qualquer pessoa para quem falar é difícil ou impossível; por exemplo, por causa de disartria, laringectomia, apraxia de fala, AVC ou lesão cerebral. Ele é otimizado para falar rápido, usar bibliotecas de frases e tocar sons no meio do texto. Oferece suporte a vários idiomas, vozes configuráveis e recursos pensados para uma comunicação eficiente.

O SimonSays usa o espaço livre da barra de tarefas do Windows e dá acesso rápido a uma dúzia de categorias de frases frequentes (e personalizáveis) com apenas alguns cliques. Como ele nunca cobre suas outras janelas, você pode continuar no navegador, nos seus documentos ou numa videochamada e falar sempre que precisar: com o teclado, com o mouse ou sem as mãos, com um rastreador ocular.

Casos de uso:
- Uso intensivo de vários aplicativos do Windows sem perder a comunicação de vista.
- Pranchas criadas por fonoaudiólogos para pessoas com deficiência de fala.
- Reuniões online com o [Virtual CABLE](https://vb-audio.com/Cable/) (veja [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)).

O SimonSays não pretende substituir programas de comunicação completos como o Mind Express, o TD Communicator, o Grid 3 ou o OptiKey, nem a avaliação e o acompanhamento profissional que costumam vir com eles. Ele faz um trabalho mais específico: falar rapidamente a partir da barra de tarefas enquanto você continua trabalhando em outros aplicativos. Ele pode funcionar junto com esses programas; por exemplo, o OptiKey ou outro software de controle ocular pode mover o cursor enquanto o SimonSays fala suas frases.

O SimonSays é de código aberto e gratuito (livre no sentido de liberdade, não só de preço), e vai continuar assim: qualquer pessoa pode usá-lo, estudá-lo, adaptá-lo e compartilhá-lo. Ele não tem anúncios e não coleta dados.

O SimonSays está chegando à versão 1.0. Antes desse marco, ele precisa de pessoas que o testem no dia a dia, com suas próprias vozes, pranchas e rastreadores oculares: se algo falhar, for incômodo ou estiver faltando, conte para nós (veja [Comentários, suporte e privacidade](#comentários-suporte-e-privacidade)). Cada comentário ajuda.

## Sumário
- [Primeiros passos](#primeiros-passos)
- [Janela de categorias](#janela-de-categorias)
- [Frases padrão](#frases-padrão)
- [Atalhos](#atalhos)
- [Configurações (F2)](#configurações-f2)
- [Olhar / Clique por fixação (F3)](#olhar--clique-por-fixação-f3)
- [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)
- [Menu do ícone da área de notificação](#menu-do-ícone-da-área-de-notificação)
- Editar pranchas (avançado):
  - [Editar categorias e frases](#editar-categorias-e-frases) (e [qual editor usar](#qual-editor-usar))
  - [Misturar voz e sons](#misturar-voz-e-sons)
  - [Personalizar a aparência (estilos da prancha e das categorias)](#personalizar-a-aparência-estilos-da-prancha-e-das-categorias)
  - [Importar e exportar categorias](#importar-e-exportar-categorias)
  - [Desfazer alterações](#desfazer-alterações)
  - [Onde o SimonSays guarda seus arquivos](#onde-o-simonsays-guarda-seus-arquivos)
- [Solução de problemas](#solução-de-problemas)
- [Comentários, suporte e privacidade](#comentários-suporte-e-privacidade)

## Primeiros passos
1. Inicie o SimonSays. Ele aparece num espaço livre da barra de tarefas do Windows e adiciona um ícone à área de notificação. A barra de tarefas pode ficar embaixo, em cima, à esquerda ou à direita da tela:
   - Quando os ícones da barra de tarefas estão centralizados, o SimonSays fica bem no início da barra (na ponta esquerda, ou em cima numa barra lateral). Quando estão alinhados à esquerda, ele fica logo antes da área de notificação.
   - Numa barra de tarefas pequena (fina), a barra do SimonSays fica mais baixa, com letra menor e os dois botões de acesso rápido lado a lado. Numa barra de tarefas à esquerda ou à direita da tela, os controles ficam empilhados um embaixo do outro.
   - A janela de categorias abre ao lado da barra de tarefas, e a seta do botão `Categorias` indica para que lado ela abre.
   - Se você mover ou redimensionar a barra de tarefas enquanto o SimonSays está em execução, ele a acompanha em poucos segundos.
2. Para falar, você pode:
   - Clicar na caixa de texto da barra (ou fixar o olhar nela): uma caixa de entrada maior abre ao lado da barra de tarefas. Digite o texto e pressione `Enter` para falar; a caixa fecha e o foco passa para `Reproduzir>`, então `Espaço` fala de novo (ou para). `Esc` fecha a caixa sem falar e leva você de volta à janela que estava usando; o texto fica na barra, onde `Reproduzir>` fala de novo.
   - Clicar em uma ou mais frases salvas na janela de categorias.
3. Durante a reprodução, o botão mostra `>` para indicar que está tocando. Você pode pressioná-lo de novo para parar na hora.

![A barra do SimonSays na barra de tarefas](img/bar.png)

Ao lado do botão `Categorias`, dois pequenos **botões de acesso rápido** dão acesso com um clique a duas coisas que, de outra forma, você alcançaria com um atalho ou pelo menu da área de notificação:

- 📂 abre uma prancha salva no seu computador; é a mesma importação de `F9`, mas começa na sua pasta de pranchas (`%LocalAppData%\SimonSays\boards\`), onde ficam as pranchas exportadas e baixadas (veja [Importar e exportar categorias](#importar-e-exportar-categorias)).
- ⚙️ abre as configurações, assim como `F2`.

Os dois podem ser ocultados com `Mostrar botões e menu de acesso rápido` nas configurações (`F2`), o que deixa mais espaço para a caixa de entrada. A mesma opção também desativa o menu de contexto (clique com o botão direito) da janela de categorias.

Se o SimonSays estiver oculto, clique no ícone dele na área de notificação para mostrá-lo de novo.

## Janela de categorias
![A janela de categorias com as frases padrão](img/categories.png)

- Abra-a com o botão `Categorias`. Redimensione-a, ou ative `Lembrar o tamanho da janela de categorias` nas configurações para manter o tamanho.
- Selecione uma categoria na lista de cima e frases na lista de baixo.
- Um clique numa frase fala automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativado; caso contrário, a frase é carregada na caixa de entrada principal sem ser falada.
- Clique duas vezes numa área vazia ou na borda da janela para redimensioná-la de forma que caibam todas as categorias e frases.
- Arraste uma área vazia da janela para movê-la, e pressione `Esc` para ocultá-la.
- Aumente e diminua o zoom com `F12` e `F11` (veja [Atalhos](#atalhos)).
- Clique com o botão direito num botão ou numa área vazia (ou pressione `Shift + F10` ou a tecla de menu) para abrir um menu com `Colar`, `Reduzir`, `Ampliar`, `Redefinir zoom`, `Ajuda`, `Configurações`, `Olhar / Clique por fixação...`, `Ocultar` e `Sair do SimonSays...`, cada um com seu atalho. A primeira entrada mostra o título da prancha (`Prancha sem título` se ela não tiver um); escolha-a para ver os créditos da prancha, o idioma e quantas categorias e frases ela tem. O menu só aparece enquanto `Mostrar botões e menu de acesso rápido` estiver ativado nas configurações.
- Num botão de categoria ou de frase, o menu também oferece `Editar...`, `Mover para a esquerda`, `Mover para a direita`, `Adicionar nova categoria...` ou `Adicionar nova frase...` e `Excluir categoria...` ou `Excluir frase...` para esse botão, sem precisar selecioná-lo antes; numa categoria, `Exportar categoria...` pergunta se você quer salvar só essa categoria ou a prancha inteira. Os atalhos `F4` a `F8` são mostrados quando o botão é a sua seleção atual, porque essas teclas atuam sobre a seleção. Numa área vazia, o menu oferece `Adicionar nova categoria...`, que adiciona uma no final; também é o jeito de começar uma prancha vazia.
- A faixa entre as categorias e as frases mostra um lembrete dos principais atalhos (ou a legenda da prancha, se ela tiver uma).

![O menu de contexto de um botão de frase](img/menu.png)

## Frases padrão
O SimonSays vem com um conjunto de categorias e frases pronto para usar em cada um dos seus 18 idiomas, ordenado para que as frases mais necessárias exijam menos seleções:

- 👋🏻 Saudações Frequentes
- 🗣️ Frases de Conversa
- 🚨 Frases de Emergência
- 🤲 Cuidado & Conforto
- 🏥 Saúde
- 🏠 Família & Casa
- 👥 Social
- ⏰ Tempo & Data
- 💼 Frases de Negócios
- ✈️ Frases de Viagem
- 🔊 Sons

Cada idioma tem suas próprias categorias e frases: quando você muda o idioma nas configurações (`F2`), a janela de categorias passa para a prancha desse idioma, e as alterações feitas num idioma não afetam os outros.

As frases são faladas com as vozes instaladas no Windows. Se não houver nenhuma voz instalada para o seu idioma, as frases aparecem mesmo assim, mas podem ser faladas com outra voz (veja [Solução de problemas](#solução-de-problemas)).

Você pode alterar as frases padrão ou recuperá-las depois; veja [Editar categorias e frases](#editar-categorias-e-frases) e [Restaurar as frases padrão](#restaurar-as-frases-padrão).

## Atalhos
- `F1`: Abrir a ajuda
- `F2`: Abrir as configurações
- `F3`: Configurações de olhar / clique por fixação
- `Enter`: Falar o texto da caixa de entrada
- `Esc`: Ocultar a janela de categorias
- `F11` / `Ctrl -`: Diminuir o zoom (janela de categorias)
- `F12` / `Ctrl +`: Aumentar o zoom (janela de categorias)
- `Ctrl + 0`: Redefinir o zoom
- `Shift + F10` ou a tecla de menu: Abrir o menu da janela de categorias (o mesmo que o clique com o botão direito)
- `Espaço` sobre `Reproduzir>`: Falar o texto de novo, ou parar enquanto toca

Os atalhos para editar categorias, frases e pranchas estão em [Editar categorias e frases](#editar-categorias-e-frases).

## Configurações (F2)
Abra as configurações (`F2`) para controlar o seguinte. As configurações têm duas colunas.

![A janela de configurações](img/settings.png)

**Coluna da esquerda**
- **Texto padrão**: ative ou desative `Inserir texto padrão automaticamente` e personalize a frase padrão.
  - `Mostrar teclado tátil`: abre o teclado virtual na tela quando a caixa de entrada abre; ele fica ao lado da caixa de entrada, do lado oposto à barra de tarefas.
- `Mostrar botões e menu de acesso rápido`: mostra os botões 📂 (abrir uma prancha) e ⚙️ (configurações) ao lado do botão `Categorias` da janela principal (veja [Primeiros passos](#primeiros-passos)) e o menu de contexto da janela de categorias (veja [Janela de categorias](#janela-de-categorias)). Desative a opção para ocultar tudo e dar mais espaço à caixa de entrada.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` depois de selecionar.
  - `Lembrar o tamanho da janela de categorias`.
  - `Redimensionar automaticamente a janela de categorias`: ajusta a janela de categorias às suas categorias e frases sempre que elas são carregadas; por exemplo, ao iniciar, depois de mudar o idioma ou depois de uma importação.
- `Olhar / Clique por fixação...`: abre as configurações de [Olhar / Clique por fixação](#olhar--clique-por-fixação-f3) por cima das configurações, que continuam abertas com suas alterações. As alterações feitas ali são salvas quando você pressiona `OK` nessa janela, mesmo que depois pressione `Cancelar` nas configurações.

**Coluna da direita**
- **Idioma**: escolha o idioma da interface (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Cada idioma também tem suas próprias categorias e frases (veja [Frases padrão](#frases-padrão)).
- **Voz**: escolha uma voz SAPI instalada que corresponda ao seu idioma.
- **Volume/Velocidade**: ajuste `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouça a voz selecionada (pelo dispositivo escolhido em `Saída de som`).
- `Saída de som`: onde o SimonSays toca sua voz e seus sons: `Padrão do Windows` (a escolha habitual) ou qualquer dispositivo conectado. Os cabos de áudio virtuais aparecem primeiro, que é o que você precisa para [reuniões online](#usar-o-simonsays-em-reuniões-online). Se o dispositivo escolhido não estiver conectado, o SimonSays toca pelo dispositivo padrão do Windows e avisa você uma vez; a escolha é mantida, aparece como `(não conectado)` e volta a ser usada assim que o dispositivo voltar.
- `Configurações de escuta...`: abre os dispositivos de gravação do Windows, onde você pode ativar **Escutar este dispositivo** para um cabo virtual (veja [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)).
- **Comportamento da reprodução**:
  - `Falar a frase imediatamente ao selecionar` (falar automaticamente com um clique).
  - `Parar reprodução anterior`: para automaticamente qualquer reprodução em andamento antes de começar uma nova frase.
  - `Aumentar o volume do SimonSays ao reproduzir`: aumenta o volume do dispositivo pelo qual o SimonSays toca (veja `Saída de som`) enquanto ele fala e restaura o nível anterior quando termina.
  - `Reduzir temporariamente outros áudios ao reproduzir`: abaixa o volume dos outros aplicativos enquanto ele fala e restaura quando termina.

**Embaixo**
- `Ajuda` (canto inferior esquerdo, ou `F1` com as configurações abertas) abre esta ajuda no idioma selecionado na lista `Idioma`; as configurações continuam abertas com suas alterações.
- `OK` salva as alterações; `Cancelar` descarta.

Por padrão, `Falar a frase imediatamente ao selecionar`, `Minimizar automaticamente a janela de categorias`, `Redimensionar automaticamente a janela de categorias`, `Aumentar o volume do SimonSays ao reproduzir` e `Reduzir temporariamente outros áudios ao reproduzir` vêm ativados, e `Parar reprodução anterior` vem desativado.

## Olhar / Clique por fixação (F3)
Ative frases e botões sem usar as mãos, mantendo o olhar (ou o cursor do mouse) sobre eles por um tempo definido. Este recurso é novo: ele precisa de testes e comentários; conte para nós como ele funciona com o seu rastreador pela opção `Enviar comentários` do menu da área de notificação.

Configurações compatíveis:
- **Qualquer software de controle ocular que mova o cursor do mouse** (Irisbond EasyClick, Tobii Dynavox Computer Control, os modos de cursor do Controle com os olhos do Windows, OptiKey): o clique por fixação segue o cursor.
- **Leitura direta do olhar (sem precisar mover o cursor)**: Irisbond Hiru (modo HID) e os rastreadores Tobii pelo software deles; verificado com o Tobii Eye Tracker 4C, o Tobii Eye Tracker 5 e o Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: o software atual dele só instala um mecanismo de 64 bits, então o SimonSays o lê por meio de um pequeno programa auxiliar, `SimonSaysGaze64.exe`, instalado ao lado dele. O auxiliar só inicia quando é necessário, não tem janela, fecha junto com o SimonSays e mantém o olhar no seu computador: ele nunca é salvo nem enviado para lugar nenhum.
- **Licença do Tobii Eye Tracker 4C e do Eye Tracker 5**: a licença de usuário final da Tobii para esses rastreadores não permite uso médico, incluindo o uso em hospitais e outros ambientes de saúde. Os dispositivos Tobii Dynavox, como o PCEye 5, têm seus próprios termos de licença.
- **Software do Tobii Eye Tracker 4C**: o SimonSays lê o 4C pelo Tobii Core. Se você conectar um Eye Tracker 5 ao mesmo computador, o Windows instala o Tobii Experience, que desativa o Tobii Core, e o 4C para de funcionar. Para usar o 4C de novo, reinstale o Tobii Core; a versão recomendada é `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, da [página de introdução da Tobii](https://gaming.tobii.com/getstarted/).

Onde o clique por fixação funciona:
- Na barra do SimonSays, na janela de categorias e nos botões de frases e categorias dela.
- Nos botões das janelas e perguntas do próprio SimonSays: as configurações, a janela Olhar / Clique por fixação, as janelas de adicionar, editar e visualizar, e todas as confirmações, como `Sim` / `Não` ou `OK` / `Cancelar`. Esses botões ficam no seu idioma e começam a responder à fixação cerca de três quartos de segundo depois que a janela abre, para que uma pergunta que aparece onde você está olhando não possa se responder sozinha.
- Quando o olhar é lido diretamente, o botão para o qual você está olhando acende como acontece sob o mouse, para você ver o que vai selecionar antes que o progresso comece. Uma piscada mantém o destaque; desviar o olhar remove o destaque.

![A janela Olhar / Clique por fixação](img/dwell.png)

Abra as configurações de Olhar / Clique por fixação (`F3`, o menu da área de notificação ou o botão `Olhar / Clique por fixação...` nas configurações) para configurar:
- **Modo de ativação**: escolha uma das opções a seguir:
  - `Automático`: detecta os dispositivos presentes e ativa o clique por fixação com o cursor do mouse ou com a detecção do olhar, conforme o caso. O software da Tobii em segundo plano (sempre em execução depois de instalado) sozinho não ativa a fixação com o cursor; um rastreador Tobii só é usado enquanto está realmente rastreando o olhar.
  - `Cursor do mouse`: sempre ativa o clique por fixação com o cursor do mouse.
  - `Rastreador ocular HID`: sempre ativa o clique por fixação com a detecção do olhar (Irisbond Hiru no modo HID, ou rastreadores Tobii). Só o seu olhar ativa os botões: enquanto nenhum olhar chega (rastreamento desligado ou rastreador desconectado), nada é ativado por fixação e o mouse funciona normalmente.
  - `Desativado`: sempre desativa o clique por fixação.
  - **Atraso de travamento**: quanto tempo, em milissegundos, seu olhar (ou o cursor) precisa ficar num botão antes que a barra de progresso comece (150 por padrão). Assim, passar o olhar pelos botões não mostra progresso nenhum; o botão é ativado depois do atraso de travamento mais o tempo de fixação. Defina 0 para o progresso começar na hora.
  - **Tempo de fixação**: a duração (em milissegundos) necessária para clicar ao olhar para um elemento (500 por padrão). Um indicador de progresso aparece enquanto a fixação acontece.
  - **Raio de tolerância**: a tolerância do olhar (em pixels) que permite pequenos movimentos do olho sem interromper a fixação.
  - **Tempo de espera**: milissegundos depois de uma ativação durante os quais o clique por fixação não pode disparar de novo (evita cliques repetidos sem querer).
  - **Cor de progresso**: a cor do indicador de progresso da fixação (por padrão, a cor de destaque do Windows).
  - **Afastar antes que a fixação ative de novo** (ativado por padrão): depois de um clique por fixação, nenhum botão pode ser ativado de novo por fixação até que o ponteiro ou o olhar se afaste desse ponto (além do raio de tolerância), ou até você desviar o olhar por um momento. Ficar sobre um botão o ativa só uma vez, então uma frase longa não é repetida antes de terminar. Os cliques do mouse não são afetados.
  - **Detectar como você usa o rastreador**: dois botões que ajudam o aplicativo a detectar se você move o cursor do mouse com um rastreador ocular ou se simplesmente clica nos botões.
  - **Detectado**: mostra o tipo de dispositivo detectado e mais informações sobre o software relacionado.
  - `Salvar relatório de diagnóstico...` (abaixo de Detectado): grava um relatório sobre o seu rastreador ocular e o software dele em `hid_dump.txt` e abre a pasta, para você enviar ao suporte.
  - Um botão `Redefinir` (embaixo à esquerda) restaura todas as configurações de fixação para os valores padrão.
  - Um botão `OK` salva as alterações; `Cancelar` descarta.

## Usar o SimonSays em reuniões online
O SimonSays pode falar diretamente numa videochamada (Teams, Zoom, Google Meet…), para que os outros participantes ouçam suas frases como se viessem do seu microfone. Você precisa de um cabo de áudio virtual, como o gratuito [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Instale o cabo de áudio virtual e reinicie o Windows se for solicitado.
2. Envie o som do SimonSays para o cabo: abra as configurações (`F2`) e escolha `CABLE Input` em `Saída de som` (os cabos virtuais aparecem primeiro). Pressione `Testar voz` para conferir: você não deve mais ouvi-lo pelos alto-falantes, porque agora o som vai para o cabo.
3. No seu aplicativo de reuniões, escolha `CABLE Output` como microfone.
4. Para você também ouvir o SimonSays, pressione `Configurações de escuta...` nas configurações. Os dispositivos de gravação do Windows abrem: selecione `CABLE Output`, abra **Propriedades → Escutar**, marque **Escutar este dispositivo**, escolha seus alto-falantes ou fones de ouvido em **Reproduzir neste dispositivo** e pressione `OK`. Você só precisa fazer isso uma vez.

O SimonSays não consegue ativar **Escutar este dispositivo** por você: é uma configuração do lado de gravação do cabo que o Windows só deixa você mesmo alterar, por isso `Configurações de escuta...` leva você direto à janela certa.

Bom saber:
- Deixe o dispositivo de reprodução padrão do Windows nos seus alto-falantes ou fones de ouvido, não em `CABLE Input`. Se **Escutar este dispositivo** tocar em "Dispositivo de reprodução padrão" enquanto esse dispositivo padrão for o cabo, o som volta para dentro do cabo.
- Se a reunião ouve suas frases mas não os efeitos sonoros (aplausos, uma campainha), é a supressão de ruído do aplicativo de reuniões que os remove; diminua ou desative a supressão de ruído no aplicativo.
- Se o cabo for desinstalado ou desconectado, o SimonSays toca pelo dispositivo padrão do Windows e avisa você uma vez; ele volta para o cabo assim que o cabo estiver disponível de novo.
- Para ter frases prontas para uma chamada, adicione a categoria **Modelo de reunião**: pressione `F9` ou o botão 📂 e abra a prancha com o nome no seu idioma (`SimonSays_Modelo_de_reunião_(Português_(Brasil)).ssc` em português do Brasil). Ela tem cumprimentos, "Você está me ouvindo?", uma breve explicação de que você fala por meio de um programa, sim / não, concordo / discordo, pedir para repetir ou falar mais devagar, e despedidas. Edite-a para adicionar frases para a sua reunião.
- Como alternativa ao passo 2, você ainda pode direcionar o SimonSays com a configuração por aplicativo do Windows (**Configurações → Sistema → Som → Mixer de volume**), deixando `Saída de som` em `Padrão do Windows`. O SimonSays só aparece nessa lista depois de ter tocado alguma coisa.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` a interface principal
- `Configurações`
- `Olhar / Clique por fixação...`
- `Web` abre a página web do SimonSays
- `Sobre` mostra a versão e os créditos
- `Enviar comentários` abre um [formulário de comentários](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador padrão
- `Sair` fecha o SimonSays, depois de perguntar

Você pode mover a janela pop-up da área de notificação e outras caixas de diálogo arrastando a barra de título; redimensione a janela de categorias arrastando as bordas, e ative `Lembrar o tamanho da janela de categorias` nas configurações para manter o tamanho.

## Editar categorias e frases
Esta seção e as seguintes são para cuidadores, fonoaudiólogos e usuários avançados que alteram as categorias, frases, sons e aparência de uma prancha. Cada alteração é salva na hora (veja [Desfazer alterações](#desfazer-alterações)).

### Qual editor usar
- **No SimonSays**: alterações do dia a dia; renomear, adicionar, mover ou excluir categorias e frases, dar a elas um emoji, adicionar sons, colar uma lista de frases ou ajustar uma cor. O resto desta seção explica como.
- **No [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: trabalho maior com pranchas, que lá é muito mais fácil: pranchas feitas com pictogramas de CAA (ele pode buscar na biblioteca de pictogramas ARASAAC), imagens de ícone personalizadas e pranchas com estilo, com cores, tamanhos e layouts. Ele é especialmente recomendado para criar uma prancha **Fitzgerald Key**, em que cada grupo de palavras tem sua própria cor. É um editor visual que abre e salva pranchas `.ssc`/`.ssz` direto no seu navegador, sem enviá-las para a internet; quando terminar, importe a prancha no SimonSays com `F9` ou com o botão 📂. Também é o jeito de usar pranchas do SimonSays em outras plataformas.

Atalhos de edição (janela de categorias):
- `F4`: Editar a seleção atual
- `F5` / `F6`: Ir para a seleção anterior / seguinte nas listas
- `F7`: Adicionar depois da seleção atual (categoria ou frase, conforme o contexto)
- `F8`: Excluir a seleção atual
- `F9`: Importar categorias
- `F10`: Exportar categorias
- `Ctrl + F4`: Editar o estilo da prancha (veja [Personalizar a aparência](#personalizar-a-aparência-estilos-da-prancha-e-das-categorias))
- `Ctrl + F8`: Excluir todas as categorias e suas frases (pede confirmação duas vezes)
- `Ctrl + V` ou `Shift + Insert`: Adicionar o texto copiado como frases, uma por parágrafo (veja [Gerenciar frases](#gerenciar-frases))
- Arrastar um arquivo `.txt`, `.ssc` ou `.ssz` para a janela: adiciona um `.txt` como nova categoria, ou importa uma prancha como com `F9` (veja [Gerenciar categorias](#gerenciar-categorias))

> **Dica:** exporte sua prancha (`F10`) antes de usar `Ctrl + F8` ou de importar uma prancha inteira (`F9`), para poder recuperá-la.

### Gerenciar categorias
- Renomear: selecione a categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover a seleção: `F5`/`F6` para ir ao item anterior/seguinte ao percorrer as listas.
- Adicionar: selecione uma categoria → (atalho: `F7`) janela `Adicionar nova categoria` → digite o nome → `OK`.
- Excluir: selecione a categoria → (atalho: `F8`) `Excluir categoria`; confirme a exclusão da categoria e das frases dela.
- Adicionar a partir de um arquivo de texto: arraste um arquivo `.txt` para a janela de categorias → é criada uma categoria com o nome do arquivo e uma frase por parágrafo (os parágrafos são separados por uma linha em branco) → confira a visualização → `OK`. Você pode soltar vários arquivos de uma vez; cada um tem sua própria visualização. Um arquivo cujo nome coincida com uma categoria existente é recusado.
- Abrir uma prancha arrastando-a: arraste um arquivo `.ssc` ou `.ssz` para a janela de categorias → o SimonSays pergunta se você quer importá-lo; com `Sim`, ele é importado como com `F9` (veja [Importar e exportar categorias](#importar-e-exportar-categorias)). Se o arquivo tiver uma prancha inteira, ela substitui a atual, e antes uma cópia da prancha atual é salva como `backup.ssz`. Você pode soltar arquivos de texto e de prancha juntos; cada um é tratado na sua vez. Abrir um arquivo `.ssc` ou `.ssz` pelo Explorador de Arquivos (clique duplo) faz a mesma pergunta.

#### Definir ou alterar o ícone de uma categoria
Cada categoria pode mostrar no botão um emoji ou um arquivo de imagem personalizado (`.ico`, `.png` ou `.jpg`). Indique o ícone na janela de adicionar ou editar com o prefixo separador `##`:

```
<ícone>##<nome da categoria>
```

- **Emoji**: digite o emoji (ou os emojis) seguido de `##` e do nome.
  - Exemplo: `🙂##Saudações`
- **Arquivo de ícone** (`.ico`, `.png`, `.jpg`): digite o nome do arquivo ou o caminho completo seguido de `##` e do nome. Um nome de arquivo sem pasta é buscado como descrito em [Ordem de busca dos arquivos de som e de ícone](#ordem-de-busca-dos-arquivos-de-som-e-de-ícone).
  - Exemplo: `C:\icons\smile.png##Saudações`
- **Sem ícone**: digite só o nome da categoria, sem o prefixo `##`.
  - Exemplo: `Saudações`

> Observação: para ícones de arquivo, só são aceitos arquivos `.ico`, `.png` e `.jpg`. As outras extensões são ignoradas e nenhum ícone é mostrado. As imagens PNG mantêm a transparência; imagens muito grandes são reduzidas automaticamente ao carregar.

### Gerenciar frases
- Editar: selecione a frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover a seleção: `F5`/`F6` para ir à frase anterior/seguinte.
- Adicionar: selecione uma frase → (atalho: `F7`) `Adicionar nova frase` → digite o texto → `OK`.
- Excluir: selecione a frase → (atalho: `F8`) `Excluir frase`; confirme.
- Adicionar várias de uma vez: copie um texto com as frases separadas por uma linha em branco → selecione uma frase (ou uma categoria) → `Ctrl + V` → confira a visualização → `OK`. As frases são adicionadas depois da frase selecionada, ou no final da categoria. As quebras de linha dentro de um parágrafo viram espaços, e os marcadores de ícone `##` e de áudio `::` funcionam como na janela `Adicionar nova frase`.
- Adicionar sons: arraste um ou mais arquivos `.wav` ou `.mp3` para a janela de categorias → confira a visualização → `OK`. Cada arquivo vira uma frase de som com o nome do arquivo, adicionada depois da frase selecionada (ou no final da categoria selecionada), e o arquivo é copiado para a pasta de recursos da prancha para acompanhá-la quando você a exportar. Um arquivo existente nunca é sobrescrito: se já houver um som diferente com o mesmo nome, o novo é salvo como `nome (2).wav`.

#### Definir ou alterar o ícone de uma frase
As frases seguem a mesma convenção de prefixo de ícone das categorias, com o separador opcional `##` antes do texto da frase:

```
<ícone>##<texto da frase>
```

- **Emoji**: `👍##Para mim está ótimo!`
- **Arquivo de ícone**: `C:\icons\check.png##Para mim está ótimo!`
- **Sem ícone**: `Para mim está ótimo!`

As frases podem incluir um arquivo de áudio (veja [Misturar voz e sons](#misturar-voz-e-sons)). Para combinar ícone, texto e áudio, use o formato:

```
<ícone>##<texto da frase>::<arquivo de áudio>
```

- Exemplo: `🔔##Atenção::alert.mp3`

O prefixo `##` e o sufixo `::` são opcionais e independentes; você pode usar qualquer combinação:
- Só prefixo de ícone: `🔔##Atenção`
- Só sufixo de áudio: `Atenção::alert.mp3`
- Os dois: `🔔##Atenção::alert.mp3`
- Nenhum: `Atenção`

## Misturar voz e sons
- Use o delimitador `♫` para inserir nomes de arquivos de áudio no texto:
  - Exemplo: `Olá ♫doorbell.mp3♫, aguarde um momento.`
  - O texto fora dos delimitadores é falado; o texto dentro é tratado como um arquivo de som e tocado naquele ponto.
- Áudio aceito: `.wav` e `.mp3`. Arquivos MIDI (`.mid`, `.midi`) são aceitos numa frase, mas não tocam; converta-os para `.wav` ou `.mp3`.
- Use caminhos absolutos ou relativos que o programa consiga acessar. Confira se os delimitadores estão em pares, para que o texto final não seja tratado como áudio.
- Para digitar `♫`, copie-o desta ajuda, ou selecione uma frase que tenha um som: o marcador `♫…♫` dela é colocado na caixa de entrada, onde você pode adicionar texto em volta.

### Sons incluídos no SimonSays
Estes sons são instalados com o SimonSays e podem ser usados pelo nome em qualquer texto ou frase; por exemplo, `♫laugh.mp3♫` na caixa de entrada, ou `Muito bem!::applause.mp3` para um botão de frase com a legenda "Muito bem!" que toca aplausos:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Ordem de busca dos arquivos de som e de ícone
Quando o nome de um arquivo de som ou de ícone não tem letra de unidade nem caminho completo, o SimonSays o busca nesta ordem e usa o primeiro que encontrar:

1. **A subpasta de recursos da prancha atual**: se o estilo da prancha definir `title` ou `resource-folder`, a própria subpasta dela dentro da pasta de recursos (veja [Personalizar a aparência](#personalizar-a-aparência-estilos-da-prancha-e-das-categorias)).
2. **Pasta de recursos**: `%LocalAppData%\SimonSays\resources\` (o local padrão para os ícones e sons das pranchas; pacotes `.ssz` importados instalam seus arquivos aqui).
3. **Pasta de dados do SimonSays**: `%LocalAppData%\SimonSays\` (mantida como alternativa para arquivos que versões anteriores colocavam aqui, antes de existir a pasta de recursos).
4. **Diretório de trabalho**: o diretório de trabalho atual do processo (só é buscado se for diferente da pasta do executável).
5. **Pasta do executável**: a pasta que contém `SimonSays.exe`.

Se o arquivo não for encontrado em nenhum desses locais, é usado o som reserva interno (um ícone simplesmente não aparece).

> **Dica:** recomendamos colocar seus arquivos de som e de ícone em `%LocalAppData%\SimonSays\resources\`; eles sobrevivem às atualizações do aplicativo e são de cada usuário. Uma prancha com um estilo `title` ou `resource-folder` ganha ali automaticamente sua própria subpasta, o que separa os arquivos dela dos de outras pranchas.

## Personalizar a aparência (estilos da prancha e das categorias)
Você pode mudar as cores, o tamanho e o estilo da janela de categorias e dos botões dela; por exemplo, para criar pranchas de comunicação com código de cores como a Fitzgerald Key. Os estilos são opcionais: sem eles, a janela mantém a aparência padrão.

> **Dica:** tudo o que vem a seguir pode ser digitado no SimonSays, o que é prático para pequenos ajustes. Para criar uma prancha inteira com estilo ou com pictogramas, o recomendado é o [SimonSays Web](https://github.com/juan-rey/simonsays-web): você vê o resultado enquanto trabalha e ele escreve esses estilos para você (veja [Qual editor usar](#qual-editor-usar)).

Um estilo é uma lista de pares `propriedade:valor;`. Os valores podem ser:
- **Cores**: hexadecimal `#RRGGBB`, por exemplo `#FFD966`.
- **Tamanhos**: um número de pixels (`96`) ou uma porcentagem do padrão (`120%`).
- **Palavras-chave**: por exemplo `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Texto**: um nome de fonte, ou o texto da legenda, do título ou dos créditos.

Tudo o que o aplicativo não reconhece (uma propriedade escrita errado ou um valor inválido) é simplesmente ignorado, então um estilo nunca estraga a janela.

### Aplicar estilo a uma categoria (janela Editar)
Selecione uma categoria e pressione `F4` (Editar); depois adicione `::` e um estilo após o nome:

```
<ícone>##<nome da categoria>::<propriedade>:<valor>;<propriedade>:<valor>;
```

- Exemplo: `🧍##Pessoas::background:#FFD966;` pinta de amarelo o botão Pessoas. A cor da categoria **também vale para as frases dentro dela**, então uma única linha colore um grupo inteiro de palavras: a base de uma prancha Fitzgerald Key.
- Um estilo de categoria pode definir `background` e `text-color` para o botão da categoria, e qualquer propriedade de frase (veja abaixo) com o prefixo `phrase-`, por exemplo `phrase-corner-radius:8;`.

### Aplicar estilo à prancha inteira (`$$board`)
A aparência geral (o fundo da janela, os separadores e os valores padrão de todos os botões de categoria e de frase) é um **estilo da prancha**. Ele é salvo com o nome de categoria reservado `$$board` (você não pode usar nomes com `$$` nas suas categorias).

- **Edite-o** pressionando `Ctrl + F4` na janela de categorias. O editor mostra um `propriedade:valor;` por linha. Ao confirmar com `OK`, o novo estilo é aplicado e o SimonSays pergunta "Deseja reverter as alterações?": escolha `OK` para voltar ao estilo anterior, ou `Cancelar` para manter o novo.
- **Consiga um** importando um arquivo de categorias com estilo (`.ssc` / `.ssz`). Quando você importa um arquivo que tem um estilo da prancha, o SimonSays pergunta antes de substituir o seu e mostra o `title` e os `credits` da prancha (se houver) na mensagem de importação. Um arquivo cujo estilo da prancha só define `title`, `credits`, `language` ou `resource-folder` não muda nada visível, então o SimonSays não pergunta: se tiver uma só categoria, ela é adicionada e sua aparência continua; se tiver uma prancha inteira, ela substitui a sua prancha, incluindo a aparência, como qualquer prancha inteira.

### Propriedades de estilo
- **Janela inteira**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (legenda mostrada entre os separadores), `title` e `credits` (mostrados quando a prancha é importada), `language` (o idioma para o qual a prancha foi feita, por exemplo `language:Portuguese (Brazil)`; o SimonSays se recusa a importar a prancha enquanto o idioma configurado for outro, e diz para qual idioma mudar nas configurações, `F2`).
- **Todos os botões de categoria** (prefixo `category-`) e **todos os botões de frase** (prefixo `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Com `icon-position` em `center`, o ícone fica no meio do botão e o texto é ocultado (uma tecla só com ícone); use `top` se quiser o ícone em cima e o nome embaixo. Se quiser texto sobre um ícone centralizado, adicione um `text-layout` explícito (por exemplo `text-layout:bottom`) e ele será mostrado nessa posição.

`text-layout` aceita uma palavra-chave horizontal (`left`/`center`/`right`) e/ou uma vertical (`top`/`middle`/`bottom`), ou `smart`. O padrão é smart: o texto é centralizado no espaço que o ícone deixa livre (ícone em cima → legenda centralizada embaixo; centralizado no botão inteiro quando não há ícone).

### Exemplo: uma prancha Fitzgerald Key com código de cores
Para colorir seis grupos de palavras, edite cada categoria (`F4`) e dê a ela uma cor de fundo. Estas são as sequências exatas que você digita na janela de edição:

```
🧍##Pessoas::background:#FFD966;
🏃##Ações::background:#93C47D;
📏##Descritivos::background:#6FABDC;
🍎##Substantivos::background:#F6B26B;
🕒##Diversos::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Cada cor também colore as frases daquela categoria, então a prancha inteira fica com código de cores. Os ajustes da prancha inteira (como colocar o emoji de cada categoria em cima com o nome embaixo, e colorir os separadores) vêm de um estilo da prancha (`Ctrl + F4`) como:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Para uma prancha Fitzgerald Key completa, com pictogramas em cada tecla e as frases organizadas por grupos de palavras, crie-a no [SimonSays Web](https://github.com/juan-rey/simonsays-web) e importe-a com `F9`.

Os estilos acompanham suas categorias: a exportação (`F10`) os inclui e a importação (`F9`) os traz junto, então uma prancha com estilo pode ser compartilhada como um único arquivo.

## Importar e exportar categorias
As pranchas são compartilhadas como arquivos `.ssc` (categorias e frases) ou `.ssz` (o mesmo, mais os arquivos de som e de ícone que elas usam, num único arquivo). Você também pode criá-las e editá-las visualmente no navegador com o [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exportar** (`F10`): quando for perguntado, escolha exportar todas as categorias ou só a selecionada; a janela de salvar abre por padrão em `%LocalAppData%\SimonSays\boards\`, mas você pode salvar em qualquer lugar. O SimonSays salva automaticamente um `.ssz` quando suas categorias ou frases usam arquivos de som ou de ícone que ele consegue encontrar (sons `.mid`/`.midi` não são incluídos). Exportar todas as categorias inclui o estilo da prancha; exportar uma categoria inclui só o estilo dela. As mensagens de sucesso ou erro aparecem no idioma selecionado.
- **Importar** (`F9`): escolha um arquivo `.ssc` ou `.ssz`.
  - Um arquivo com **várias categorias ou com um estilo da prancha** é uma prancha inteira: ele **substitui todas as categorias e frases do idioma atual**. Antes, sua prancha anterior é salva como `backup.ssz` (veja [Desfazer alterações](#desfazer-alterações)).
  - Um arquivo com **uma só categoria** é adicionado à sua prancha; se já existir uma categoria com o mesmo nome, você será perguntado se quer sobrescrevê-la. O mesmo vale quando o arquivo só dá nome à prancha dele (`title`, `credits` ou `language`, sem cores, tamanhos nem fontes): esses dados nunca substituem sua prancha nem a aparência dela.
  - Os arquivos de som e de ícone de um `.ssz` são instalados na pasta de recursos, e a janela de categorias se ajusta à prancha importada. As mensagens de sucesso ou erro aparecem no idioma selecionado.

## Desfazer alterações
O SimonSays salva cada alteração na hora e não tem um comando geral de Desfazer. Estes são os jeitos de voltar atrás:

- **Desfazer uma importação**: antes de uma importação substituir sua prancha, o SimonSays salva a prancha anterior como `backup.ssz` em `%LocalAppData%\SimonSays\boards\`. Para desfazer a importação, pressione `F9` e escolha `backup.ssz`: sua prancha anterior volta como estava, categorias e aparência, sem mais perguntas. Só a última prancha substituída é guardada: cada nova importação de uma prancha inteira sobrescreve a cópia.
- **Desfazer uma alteração no estilo da prancha**: depois de editar o estilo da prancha com `Ctrl + F4`, o SimonSays pergunta "Deseja reverter as alterações?"; escolha `OK` para voltar ao estilo anterior.
- **Desfazer uma alteração de categoria ou de frase**: as alterações feitas com `F4`, `F7` ou `F8` não podem ser desfeitas automaticamente. Antes de fazer grandes alterações numa prancha importante para você, exporte-a (`F10`); importar esse arquivo depois (`F9`) leva você de volta àquele ponto.

### Restaurar as frases padrão
O SimonSays nunca sobrescreve suas próprias categorias e frases, então as novas frases padrão de uma atualização só aparecem nos idiomas que você ainda não usou. Você pode voltar às categorias e frases internas a qualquer momento; por exemplo, para recomeçar ou para pegar as frases novas de uma atualização. Isso **substitui a prancha inteira do idioma atual**:

1. Clique no botão 📂 ao lado de `Categorias` (ou pressione `F9` na janela de categorias). A pasta de pranchas abre.
2. Escolha a prancha de frases padrão do seu idioma; em português do Brasil, `SimonSays_frases_padrão_(Português_(Brasil)).ssc` (nos outros idiomas ela tem a mesma forma, naquele idioma; por exemplo, `SimonSays_default_phrases_(English).ssc`).
3. Confirme a importação. Antes, sua prancha atual é salva como `backup.ssz`, então você ainda pode recuperá-la (veja [Desfazer alterações](#desfazer-alterações)).

Essa prancha sempre tem as frases padrão da versão do SimonSays que você está usando: o SimonSays a atualiza na pasta de pranchas ao iniciar e ao mudar de idioma. Se quiser manter sua própria prancha, exporte-a antes (`F10`) com o nome que preferir; o `backup.ssz` é sobrescrito na próxima importação.

Se o arquivo não estiver na pasta de pranchas, há um último recurso: saia do SimonSays (menu da área de notificação → `Sair`), abra o Editor do Registro (`regedit`), exclua a chave `Portuguese (Brazil)` em `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` e inicie o SimonSays de novo. As categorias e frases padrão desse idioma são criadas de novo, sem cópia de segurança.

Para `Portuguese (Brazil)`: se também existir uma chave `Portuguese`, o SimonSays copia essa prancha para `Portuguese (Brazil)` em vez de criar as frases padrão brasileiras, então exclua também a chave `Portuguese` (exporte-a antes se você a usa).

## Onde o SimonSays guarda seus arquivos
- **As categorias, as frases e as configurações** são salvas no registro do Windows, em `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (a prancha de cada idioma em `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: seus arquivos de som e de ícone, e os instalados a partir de pranchas `.ssz`; uma prancha cujo estilo define `title` ou `resource-folder` tem sua própria subpasta.
- `%LocalAppData%\SimonSays\boards\`: a pasta padrão das pranchas exportadas, a cópia automática `backup.ssz` e as pranchas que vêm com o SimonSays: as frases padrão de cada idioma, e categorias prontas que você pode adicionar à sua prancha com `F9` ou com o botão 📂: Sim / Não, Dor e corpo, Soletrar, Jogos, Perguntas, Cuidados diários, um Modelo de reunião e um Teste de audição, com o nome no seu idioma, e `Spelling_NATO`, `Alert_tones` e `Morse_code` para qualquer idioma. Cada uma adiciona uma categoria sem substituir sua prancha. As pranchas do seu idioma são copiadas para lá ao iniciar e quando você muda o idioma nas configurações, sempre que a prancha padrão do seu idioma estiver faltando ou tiver mudado, substituindo as cópias da pasta; por isso, salve com outro nome as pranchas que você editar.
- `%LocalAppData%\SimonSays\debug\`: arquivos de diagnóstico (como `hid_dump.txt`) que o suporte pode pedir.

Para abrir uma dessas pastas, cole o caminho dela (por exemplo `%LocalAppData%\SimonSays`) na barra de endereços do Explorador de Arquivos.

## Solução de problemas
- **A inicialização da voz falha**: confira se o SAPI está instalado e se há uma voz compatível.
- **Não há voz para o meu idioma, ou as frases são faladas com outro sotaque**: o SimonSays usa as vozes instaladas no Windows. Adicione uma voz para o seu idioma nas configurações do Windows (**Hora e idioma → Fala → Gerenciar vozes**) e depois escolha-a nas configurações do SimonSays (`F2`). Algumas vozes "naturais" mais recentes do Windows podem não estar disponíveis para outros aplicativos.
- **Os sons não tocam**: confira o caminho e a extensão do arquivo, e se os delimitadores `♫` estão em pares.
- **A barra de tarefas não se oculta mais automaticamente com o SimonSays aberto**: é intencional. O SimonSays fica na barra de tarefas, então a mantém visível enquanto está em execução e volta a ativar "Ocultar automaticamente a barra de tarefas" quando você sai dele. Se o SimonSays foi fechado de forma inesperada (por exemplo, numa queda de energia), a opção volta a ser ativada na próxima vez que você sair do SimonSays.
- **O botão Widgets cobre a barra do SimonSays** (Windows 11): desative os Widgets em **Configurações → Personalização → Barra de tarefas** do Windows. O SimonSays oferece abrir essa página uma vez, na primeira vez que percebe os Widgets.
- **A janela de categorias está pequena ou grande demais**: clique duas vezes numa área vazia ou na borda da janela para ajustá-la ao conteúdo, ou use o zoom com `F11` / `F12`.
- **Minhas categorias ou frases sumiram**: confira o idioma nas configurações (`F2`); cada idioma tem sua própria prancha. Se uma importação substituiu sua prancha, veja [Desfazer alterações](#desfazer-alterações).
- **"O SimonSays não conseguiu salvar suas categorias e frases"**: sua alteração continua aparecendo, mas pode ser perdida ao fechar o SimonSays. Exporte sua prancha com `F10` imediatamente para guardar uma cópia; se acontecer de novo, informe-nos. A mensagem aparece só uma vez, não a cada alteração, para nunca atrapalhar o uso da prancha.
- **Erros de importação ou exportação**: confira as permissões do arquivo e o local no disco; tente de novo.
- **As mudanças de idioma ou de voz não são aplicadas**: abra as configurações de novo e confirme a seleção. Se as mudanças continuarem sem ser aplicadas, reinicie o SimonSays e tente de novo.
- **Informar problemas com o rastreador ocular**: se o suporte pedir um diagnóstico, abra a janela Olhar / Clique por fixação (`F3`) e clique em `Salvar relatório de diagnóstico...`. O relatório (`hid_dump.txt`) é salvo em `%LocalAppData%\SimonSays\debug\` e essa pasta abre. Ele só é criado quando você pede.

## Comentários, suporte e privacidade
- A janela `Sobre` (menu da área de notificação) mostra a versão, a descrição e os direitos autorais.
- Compartilhe sugestões, problemas ou como o SimonSays funciona com o seu rastreador ocular pela opção `Enviar comentários` do menu da área de notificação, que abre um [formulário de comentários](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Para informar bugs e obter suporte técnico, visite o [repositório do SimonSays no GitHub](https://github.com/juan-rey/SimonSays).
- O SimonSays é livre e de código aberto, sob a Open Software License 3.0 (OSL-3.0). Ele não mostra anúncios e não coleta seus dados.
