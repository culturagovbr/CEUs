<html>
    <head>
        <title>V�nculos com endere�o divergente</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <script type="text/javascript">

    function editar(cod){
    	window.opener.formulario.act.value = 'alterarvinculo';
        window.opener.formulario.vceid.value = cod;

        window.opener.formulario.submit();
        window.close();
    }

	</script>
	 <body>
	  <?php
	  		monta_titulo( 'V�nculos com endere�o divergente', 'Para visualizar as informa��es clique no �cone &nbsp; <img style="border:0px; vertical-align:middle;" src="/imagens/consultar.gif">' );

	  		$mVinculoCeu = new VinculoCeu();
	  		$mVinculoCeu->limparVinculoSession();

		    $arConfig = array("style"           => "width:95%;",
		                      "totalLinha"      => false,
		                      "totalRegistro"   => true);
		    $arCabecalho = array( "Informa��es Principais" => array("A��o", "Identificador", "CEU", "UF", "Munic�pio","Situa��o"), "Informa��es de Endere�o" => array('CEP',"Logradouro","N�mero","Complemento","Bairro","Munic�pio/UF"));


		    $arParamCol[0]  = array('type' => Lista::TYPESTRING);

		    $acao = '<center>
		    			<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {vceid} )">&nbsp;
		             </center>';


		    $rs = $mVinculoCeu->listaPopUpCeu( $_REQUEST['ceuid'] );

		    $oLista = new Lista($arConfig);
		    $oLista->setCabecalho( $arCabecalho );
		    $oLista->setCorpo( $rs, $arParamCol );
		    $oLista->setAcao( $acao );
		    $oLista->show();
	  ?>
	 </body>
</html>