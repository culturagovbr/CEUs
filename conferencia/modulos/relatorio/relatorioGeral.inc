<?php
set_time_limit(120);

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
    $existe_rel = 0;
    $sql = sprintf("SELECT
                        prtid
                    FROM
                        public.parametros_tela
                    WHERE
                        prtdsc = '%s'",
                    $_REQUEST['titulo']);
    $existe_rel = $db->pegaUm( $sql );

    if ($existe_rel > 0){
        $sql = sprintf("UPDATE public.parametros_tela SET
                            prtdsc = '%s',
                            prtobj = '%s',
                            prtpublico = 'FALSE',
                            usucpf = '%s',
                            mnuid = %d
                        WHERE
                            prtid = %d",
                        $_REQUEST['titulo'],
                        addslashes( addslashes( serialize( $_REQUEST ) ) ),
                        $_SESSION['usucpf'],
                        $_SESSION['mnuid'],
                        $existe_rel);
    }else{
        $sql = sprintf("INSERT INTO public.parametros_tela (
                            prtdsc, prtobj, prtpublico, usucpf, mnuid
                        ) VALUES (
                            '%s', '%s', %s, '%s', %d
                        )",
                    $_REQUEST['titulo'],
                    addslashes( addslashes( serialize( $_REQUEST ) ) ),
                    'FALSE',
                    $_SESSION['usucpf'],
                    $_SESSION['mnuid']);
    }
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Opera��o realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioGeral&acao=A';
         </script>");
}

// transforma consulta em p�blica
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
    $sql = sprintf("UPDATE public.parametros_tela SET
                        prtpublico = CASE
                                        WHEN prtpublico = true
                                        THEN false
                                     ELSE
                                        true
                                     END
                    WHERE
                        prtid = %d",
                    $_REQUEST['prtid']);
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Opera��o realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioGeral&acao=A';
         </script>");
}
// FIM - transforma consulta em p�blica

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
    $sql = sprintf("DELETE FROM
                        public.parametros_tela
                    WHERE
                        prtid = %d",
                    $_REQUEST['prtid']);
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Opera��o realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioGeral&acao=A';
         </script>");
}
// FIM - remove consulta

// remove flag de submiss�o de formul�rio
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ){
    unset( $_REQUEST['form'] );
}
// FIM remove flag de submiss�o de formul�rio

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
    if ( $_REQUEST['prtid'] ){
        $sql = sprintf( "SELECT
                            prtobj
                         FROM
                            public.parametros_tela
                         WHERE
                            prtid = " . $_REQUEST['prtid'] );
        $itens = $db->pegaUm( $sql );
        $dados = unserialize( stripslashes( stripslashes( $itens ) ) );
        $_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
        unset( $_REQUEST['salvar'] );
    }
    include "relatorioGeralResult.inc";
    exit;

}

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
    $sql = sprintf( "SELECT
                        prtobj
                     FROM
                        public.parametros_tela
                     WHERE
                        prtid = ".$_REQUEST['prtid'] );
    $itens = $db->pegaUm( $sql );

    $dados = unserialize( stripslashes( stripslashes( $itens ) ) );
    extract( $dados );
    $_REQUEST = $dados;
    unset( $_REQUEST['form'] );
    unset( $_REQUEST['pesquisa'] );
    $titulo = $_REQUEST['titulo'];

    $agrupador2 = array();

    if ( $_REQUEST['agrupador'] ){
        foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
            array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
        }
    }
}

if ( isset( $_REQUEST['pesquisa'] ) || isset( $_REQUEST['tipoRelatorio'] ) ){
    include "geralResult.inc";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relat�rio Geral";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<!-- CARREGANDO DADOS AJAX -->
<div id="loader-container" style="display:none;">
    <div id="loader">
        <img src="../imagens/wait.gif" border="0" align="middle">
        <span>Aguarde! Carregando Dados...</span>
    </div>
</div>

<form action="" method="post" name="filtro">
    <input type="hidden" name="form" value="1"/>
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relat�rio p�blico ou privado -->
    <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relat�rio p�blico ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relat�rio -->
    <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relat�rio j� gravado -->

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita">T�tulo</td>
            <td>
                <?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Agrupadores</td>
            <td>
                <?php

                    // In�cio dos agrupadores
                    $agrupador = new Agrupador('filtro','');

                    $agrupador->width = 345;

                    // Dados padr�o de destino (nulo)
                    $destino = isset( $agrupador2 ) ? $agrupador2 : array();

                    // Dados padr�o de origem
                    $origem = array(
                        array(
                            'codigo'    => 'pcnid',
                            'descricao' => 'Identificador da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'mconome',
                            'descricao' => 'Edi��o da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'tpcnome',
                            'descricao' => 'Tipo da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'ogcnome',
                            'descricao' => '�rg�o Gestor em Cultura'
                        ),
                        array(
                            'codigo'    => 'cgonome',
                            'descricao' => 'Cargo'
                        ),
                        array(
                            'codigo'    => 'endmundescricao',
                            'descricao' => 'Munic�pio'
                        ),
                        array(
                            'codigo'    => 'endestuf',
                            'descricao' => 'Estado(UF)'
                        )
                    );

                    // exibe agrupador
                    $agrupador->setOrigem( 'naoAgrupador', null, $origem );
                    $agrupador->setDestino( 'agrupador', null, $destino );
                    $agrupador->exibir();
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Colunas</td>
            <td>
                <?php
                    unset($agrupador, $destino, $origem);
                    // In�cio das colunas
                    $agrupador = new Agrupador('filtro','');

                    $agrupador->width = 345;

                    // Dados padr�o de destino (nulo)
                    $destino = isset( $agrupador2 ) ? $agrupador2 : array();

                    // Dados padr�o de origem
                    $origem = array(
                        array(
                            'codigo'    => 'mconome',
                            'descricao' => 'Edi��o da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'tpcnome',
                            'descricao' => 'Tipo da Confer�ncia'
                        ),array(
                            'codigo'    => 'mundescricao',
                            'descricao' => 'Munic�pio/UF'
                        ),
                        array(
                            'codigo'    => 'pcntitulo',
                            'descricao' => 'T�tulo da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'pcnperiodo',
                            'descricao' => 'Per�odo'
                        ),
                        array(
                            'codigo'    => 'esddsc',
                            'descricao' => 'Situa��o da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'ogcnome',
                            'descricao' => '�rg�o Gestor em Cultura'
                        ),
                        array(
                            'codigo'    => 'cgonome',
                            'descricao' => 'Cargo'
                        ),
                        array(
                            'codigo'    => 'endcep',
                            'descricao' => 'CEP Realiza��o'
                        ),
                        array(
                            'codigo'    => 'endlog',
                            'descricao' => 'Logradouro Realiza��o'
                        ),
                        array(
                            'codigo'    => 'endnum',
                            'descricao' => 'N�mero Realiza��o'
                        ),
                        array(
                            'codigo'    => 'endmundescricaocompleto',
                            'descricao' => 'Munic�pio/UF Realiza��o'
                        ),
                        array(
                            'codigo'    => 'possuidecretoconvocacao',
                            'descricao' => 'Decreto de Convoca��o da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'possuidecretoconvocacaodiario',
                            'descricao' => 'Publica��o do Decreto de Convoca��o da Confer�ncia em Di�rio Oficial'
                        ),
                        array(
                            'codigo'    => 'possuiportarianomeacao',
                            'descricao' => 'Portaria de Nomea��o do Coordenador'
                        ),
                        array(
                            'codigo'    => 'cnfdtpubconvocacao',
                            'descricao' => 'Data de Publica��o da Convoca��o'
                        ),
                        array(
                            'codigo'    => 'cnfdtrealizacao',
                            'descricao' => 'Data de Realiza��o da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'cnfsegmento',
                            'descricao' => 'Segmento ou Entidade que Convocou a Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'cnfqtdptcconferencia',
                            'descricao' => 'Quantidade de participandes - Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'cnfqtdptcpreconferencia',
                            'descricao' => 'Quantidade de participandes - Pr�-Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'cnfqtddelegeleitos',
                            'descricao' => 'Quantidade de participandes - Delegados Eleitos'
                        ),
                        array(
                            'codigo'    => 'cnfqtddeleganteriores',
                            'descricao' => 'Quantidade de participandes - Delegados Municipais'
                        ),
                        array(
                            'codigo'    => 'cnfqtddelegnatos',
                            'descricao' => 'Quantidade de participandes - Delegados Natos'
                        ),
                        array(
                            'codigo'    => 'totaldelegadospresentes',
                            'descricao' => 'Quantidade de participandes - Total de Delegados Presentes'
                        ),
                        array(
                            'codigo'    => 'cnfqtdpublico',
                            'descricao' => 'Quantidade de participandes - P�blico Total da Confer�ncia'
                        ),
                        array(
                            'codigo'    => 'cnfqtdsociedadecivil',
                            'descricao' => 'Delegados Eleitos Nesta Etapa - Sociedade Civil'
                        ),
                        array(
                            'codigo'    => 'cnfqtdpoderpublico',
                            'descricao' => 'Delegados Eleitos Nesta Etapa - Poder P�blico'
                        ),
                        array(
                            'codigo'    => 'cnfinfadc',
                            'descricao' => 'Informa��es Adicionais'
                        )
                    );

                    // exibe agrupador
                    $agrupador->setOrigem( 'naoColuna', null, $origem );
                    $agrupador->setDestino( 'coluna', null, $destino );
                    $agrupador->exibir();
                ?>
            </td>
        </tr>
        </table>

        <!-- OUTROS FILTROS -->

        <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
            <tr>
                <td onclick="javascript:onOffBloco( 'outros' );">
                    <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                    Relat�rios Gerenciais
                    <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
                </td>
            </tr>
        </table>

        <div id="outros_div_filtros_off"></div>

        <div id="outros_div_filtros_on" style="display:none;">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Relat�rios:</td>
                        <?php

                            if( $db->testa_superuser() ){
                                $bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
                            }

                            $sql = sprintf("SELECT
                                                CASE WHEN prtpublico = true AND usucpf = '%s'
                                                    THEN '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao
                                            FROM
                                                public.parametros_tela
                                            WHERE
                                                mnuid = %d AND
                                                prtpublico = TRUE",
                                $_SESSION['usucpf'],
                                $_SESSION['mnuid'],
                                $_SESSION['usucpf']
                            );

                            $cabecalho = array('A��o', 'Nome');
                        ?>
                        <td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
                        </td>
                    </tr>
            </table>
        </div>
        <script language="javascript">  //alert( document.formulario.agrupador_combo.value );   </script>

        <!-- FIM OUTROS FILTROS -->



        <!-- MINHAS CONSULTAS -->
        <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
            <tr>
                <td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
                    <!-- -->
                    <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                    Minhas Consultas
                    <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
                </td>
            </tr>
        </table>
        <div id="minhasconsultas_div_filtros_off">
        </div>
        <div id="minhasconsultas_div_filtros_on" style="display:none;">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                        <?php

                            $sql = sprintf(
                                "SELECT
                                    CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                                 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                    END as acao,
                                    '' || prtdsc || '' as descricao
                                 FROM
                                    public.parametros_tela
                                 WHERE
                                    mnuid = %d AND usucpf = '%s'",
                                $_SESSION['mnuid'],
                                $_SESSION['usucpf']
                            );

                            $cabecalho = array('A��o', 'Nome');

                        ?>
                        <td>
                            <?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
                        </td>
                    </tr>
            </table>
        </div>
        <!-- FIM MINHAS CONSULTAS -->

        <!-- MEUS FILTROS -->
        <table class="tabela" style=" border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <?php
            /********************************************************************
             * Edi��o da Confer�ncia */
            $stSql = "  SELECT DISTINCT
                            mcoid AS codigo,
                            mconome AS descricao
                        FROM
                            conferencia.momentoconferencia
                        WHERE
                            mcostatus = 'A'
                        ORDER BY
                            2";

            mostrarComboPopup( 'Edi��o da Confer�ncia', 'mcoid',  $stSql, $stSqlCarregados, 'Selecione a(s) edi��o(�es) da confer�ncia' );

            /********************************************************************
             * Tipo de Confer�ncia */
            $stSql = "  SELECT DISTINCT
                            tpcid AS codigo,
                            tpcsigla || ' - ' || tpcnome AS descricao
                        FROM
                            conferencia.tipoconferencia
                        WHERE
                            tpcstatus = 'A'
                        ORDER BY
                            2";

            mostrarComboPopup( 'Tipo de Confer�ncia', 'tpcid',  $stSql, $stSqlCarregados, 'Selecione o(s) tipo(s) de confer�ncia' );

            /********************************************************************
             * Estado */
            $stSql =  "SELECT
                            estuf AS codigo,
                            estuf || ' - ' || estdescricao AS descricao
                        FROM
                            territorios.estado
                        ORDER BY
                            2";

            mostrarComboPopup( 'Estado', 'estuf',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' );

            /********************************************************************
             * Munic�pio */
            $stSql =  "SELECT
                            muncod AS codigo,
                            estuf ||' / '|| mundescricao AS descricao,
                            estuf AS ordem
                       FROM
                            territorios.municipio";

            mostrarComboPopup( 'Munic�pio', 'muncod',  $stSql, $stSqlCarregados, 'Selecione o(s) Munic�pio(s)',null,null,false,false,null,array('estuf') );

            /********************************************************************
             * Org�o Gestor em cultura */

            $stSql =  "SELECT
                            ogcid AS codigo,
                            ogcnome AS descricao
                       FROM
                            conferencia.orgaogestorcultura
                       WHERE
                            ogcstatus = 'A'";

            mostrarComboPopup( 'Org�o Gestor em cultura', 'ogcid',  $stSql, $stSqlCarregados, 'Selecione o(s) Org�o(�os) Gestor(ores) em cultura');

            ?>

            <tr>
                <td class="SubTituloDireita">
                    Adicionar dados complementares?
                </td>
                <td>
                    <input type="radio" name="dadoscomplementares" id="dadoscomplementares-t" <?php echo ($dadoscomplementares == 't' || empty($dadoscomplementares) ) ? ' checked="checked" ' : "" ?> value="t" /> <label for="dadoscomplementares-t" style="cursor:pointer;"> Sim</label>
					<input type="radio" name="dadoscomplementares" id="dadoscomplementares-f" <?php echo $dadoscomplementares == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="dadoscomplementares-f" style="cursor:pointer;"> N�o</label>
                </td>
            </tr>
        </table>

        <table class="tabela" style="border-top:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <tr>
                <td bgcolor="#CCCCCC" width="195"></td>
                <td bgcolor="#CCCCCC">
                    <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
                    <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
                    <input type="button" value="Salvar Consulta" onclick="exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>

                </td>
            </tr>
        </table>
</form>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

    function formataNumero(elemento){
        var valor = elemento.value;
        var valorFormatado = new Number(valor.replace(",",".") );

        if(valorFormatado > 100){
            elemento.value = '100,00';
        }
    }

    function exibeRelatorioGeral(tipo){

        var formulario = document.filtro;
        var agrupador  = document.getElementById( 'agrupador' );
        var coluna     = document.getElementById( 'coluna' );

        // Tipo de relatorio
        formulario.pesquisa.value='1';

        prepara_formulario();
        selectAllOptions( formulario.agrupador );
        selectAllOptions( formulario.coluna );

        if ( tipo == 'relatorio' ){

            formulario.action = '?modulo=relatorio/relatorioGeral&acao=A';
            window.open( '', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            formulario.target = 'relatorio';

        }else {
            if ( tipo == 'salvar' ){

                if ( formulario.titulo.value == '' ) {
                    alert( '� necess�rio informar o t�tulo do relat�rio!' );
                    formulario.titulo.focus();
                    return false;
                }

                if ( !agrupador.options.length ){
                    alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
                    agrupador.focus();
                    return false;
                }

                if ( !coluna.options.length ){
                    alert( 'Favor selecionar ao menos um item de coluna para o resultado!' );
                    coluna.focus();
                    return false;
                }

                var nomesExistentes = new Array();
                <?php
                    $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
                    $nomesExistentes = $db->carregar( $sqlNomesConsulta );
                    if ( $nomesExistentes ){
                        foreach ( $nomesExistentes as $linhaNome )
                        {
                            print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
                        }
                    }
                ?>
                var confirma = true;
                var i, j = nomesExistentes.length;
                for ( i = 0; i < j; i++ ){
                    if ( nomesExistentes[i] == formulario.titulo.value ){
                        confirma = confirm( 'Deseja alterar a consulta j� existente?' );
                        break;
                    }
                }
                if ( !confirma ){
                    return;
                }
                formulario.action = '?modulo=relatorio/relatorioGeral&acao=A&salvar=1';
                formulario.target = '_self';

            }else if( tipo == 'exibir' || tipo == 'xls' || tipo == 'mapa'){
                if(tipo != 'mapa'){
                    if ( !agrupador.options.length ){
                        alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
                        agrupador.focus();
                        return false;
                    }

                    if ( !coluna.options.length ){
                        alert( 'Favor selecionar ao menos um item de coluna para o resultado!' );
                        coluna.focus();
                        return false;
                    }

                    selectAllOptions( agrupador );
                    selectAllOptions( coluna );
                }

                if(tipo == 'xls'){
                	formulario.action = '?modulo=relatorio/relatorioGeral&acao=A&tipoRelatorio=xls';
                }else if(tipo == 'mapa'){
                	formulario.action = '?modulo=relatorio/relatorioGeral&acao=A&tipoRelatorio=mapa';
                }else{
                	formulario.action = '?modulo=relatorio/relatorioGeral&acao=A';
                }
                formulario.target = 'resultadoFtGeral';
                var janela = window.open( '', 'resultadoFtGeral', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
                janela.focus();
            }
        }

        formulario.submit();

    }

    function tornar_publico( prtid ){
        document.filtro.publico.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

    function excluir_relatorio( prtid ){
        document.filtro.excluir.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

//    function carregar_consulta( prtid ){
//        document.filtro.carregar.value = '1';
//        document.filtro.prtid.value = prtid;
//        document.filtro.target = '_self';
//        document.filtro.submit();
//    }

    function carregar_relatorio( prtid ){
        document.filtro.prtid.value = prtid;
        exibeRelatorioGeral( 'relatorio' );
    }

    /* Fun��o para substituir todos */
    function replaceAll(str, de, para){
        var pos = str.indexOf(de);
        while (pos > -1){
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }
    /* Fun��o para adicionar linha nas tabelas */

    /**
     * Alterar visibilidade de um bloco.
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco( bloco )
    {
        var div_on = document.getElementById( bloco + '_div_filtros_on' );
        var div_off = document.getElementById( bloco + '_div_filtros_off' );
        var img = document.getElementById( bloco + '_img' );
        var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo( campo )
    {
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>
