<?php 
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:migradados.inc
   Finalidade: permitir migrar os dados 
   */
// importa��o de responsabilidades

  
if ($_REQUEST['migrap'])
{
	/*
// migrar a��o
//1 - procurar na tabela ppaprograma os dados
// 2 - verificar na tabela ppaprograma_proposta se o programao j� existe. Se n�o existir, ent�o incluir.

    $sql="select p.* from elabrev.ppaprograma p where p.prsano='".$_REQUEST['anoorigem']."'";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em vari�veis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=addslashes($v);
     		$sql = "select prgcod from elabrev.ppaprograma_proposta where prgcod='$prgcod' and prsano='$prsano'";            
     		$dados_origem= $db->recuperar($sql);
     		if (! $dados_origem['prgcod'] )     		
     		{ 
     			// ent�o n�o existe     			
     			$sql = "insert into elabrev.ppaprograma_proposta (prgcod,prgid,tpocod,tprcod,orgcod,prgdsc,prgmesinicio,prganoinicio,prgmestermino,prganotermino,prgdscobjetivo,prgdscpublicoalvo,prgdscjustificativa,prgdscestrategia,prgvlrprograma,prgsntemporario,prgsnpadronizado,prgobs,prsano) 
     			values('$prgcod',$prgid,'$tpocod','$tprcod','$orgcod','$prgdsc','$prgmesinicio','$prganoinicio','$prgmestermino','$prganotermino','$prgdscobjetivo','$prgdscpublicoalvo','$prgdscjustificativa','$prgdscestrategia',$prgvlrprograma,'$prgsntemporario','$prgsnpadronizado','$prgobs','$prsano')";
       	        $saida=$db->executar($sql);	
            }
	    }
     }
     $db->commit();
     
     // a��es
// migrar a��o
//1 - procurar na tabela ppaacao os dados
// 2 - verificar na tabela ppaacao_proposta se a acao j� existe. Se n�o existir, ent�o incluir.
//3 - ao final, gerar as unidades que tem aquela a��o e guardar na tablea unidade_acao
    $sql="select a.* from elabrev.ppaacao a where a.prsano='".$_REQUEST['anoorigem']."'";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em vari�veis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=addslashes($v);     		
     		$sql = "select acacod from elabrev.ppaacao_proposta where acacod='$acacod' and prsano='$prsano'";            
     		$dados_origem= $db->recuperar($sql);    		
     		if (! $dados_origem['acacod'] )     		
     		{ 
     		    $sql = "select prgid from elabrev.ppaprograma_proposta where prgcod='$prgcod' and prsano='$prsano'";            
     		    $dados_prg= $db->recuperar($sql);
     		    $prgid=$dados_prg['prgid'];
     			// ent�o n�o existe     			
     			$sql = "insert into elabrev.ppaacao_proposta (prgcod,acacod,funcod,sfucod,taccod,procod,acadscproduto,unmcod,tincod,esfcod,acadsc,acafinalidade,acadescricao,acabaselegal,acarepercfinanceira,acavlrrepercfinanceira,acasnpadronizada,acasnsetpadronizada,acasnmedireta,acasnmedesc,acasnmelincred,acasntransfobrigatoria,acasntransfvoluntaria,acasntransfoutras,acasndespesaobrigatoria,acasnbloqueioprogramacao,acadetalhamento,acamesinicio,acaanoinicio,acamestermino,acaanotermino,acavlrcustototal,acaqtdcustototal,acavlrcustoateanoanterior,acaqtdateanoanterior,acavlrcustoprevistoanocorrente,acaqtdprevistoanocorrente,acaordemprioridade,acaoobs,prgid,prsano) 
     			values('$prgcod','$acacod','$funcod','$sfucod','$taccod',$procod,'$acadscproduto',$unmcod,$tincod,$esfcod,'$acadsc','$acafinalidade','$acadescricao','$acabaselegal','$acarepercfinanceira','$acavlrrepercfinanceira','$acasnpadronizada','$acasnsetpadronizada','$acasnmedireta','$acasnmedesc','$acasnmelincred','$acasntransfobrigatoria','$acasntransfvoluntaria','$acasntransfoutras','$acasndespesaobrigatoria','$acasnbloqueioprogramacao','$acadetalhamento','$acamesinicio','$acaanoinicio','$acamestermino','$acaanotermino',$acavlrcustototal,$acaqtdcustototal,$acavlrcustoateanoanterior,$acaqtdateanoanterior,$acavlrcustoprevistoanocorrente,$acaqtdprevistoanocorrente,$acaordemprioridade,'$acaoobs',$prgid,'$prsano')";
   			
       	        $saida=$db->executar($sql);	
	         }
	    }
     }     
     $db->commit();
    // aqui fa�o a checagem da tabela unidade_acao
    // para cada a��o, verifico as unidades que ela atende. descubro o acaid em ppaacao_proposta e o c�digo da unidade e gravo na tabela unidade_acao
     $sql="select a.acaid, a.acacod,a.prsano from elabrev.ppaacao_proposta a where a.prsano='".$_REQUEST['anoorigem']."'";
	 $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas >= 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em vari�veis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
     		// achos as unidades que s�o atendidas   		
     		$sql = "select unicod from elabrev.ppaacao where acacod='$acacod' and prsano='$prsano'"; 
     		$RS2 = $db->record_set($sql);
	        $nlinhas2 = $db->conta_linhas($RS2);
	        if ($nlinhas2 >= 0)
	        {
	        	for ($ii=0; $ii<=$nlinhas2;$ii++)
   		       {
   		       	  $res = $db->carrega_registro($RS2,$ii);
     		      // a linha abaixo transforma em vari�veis todos os campos do array
     		      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

     		    //  print 'unicod='.$unicod.'-----uniid='.$uniid.'<br>';
     		      $sql = "select acaid from elabrev.unidade_acao where acaid=$acaid and unicod='$unicod'";            
     		      $dados_uni= $db->recuperar($sql);
     		      if (! $dados_uni['acaid'])
     		      {
     		      	$sql = "insert into unidade_acao (unicod,acaid) values ('$unicod',$acaid)";
     		      	$saida=$db->executar($sql);	
     		      }  
   		       }
	        }	
   		}
	 } 
     
     $db->commit();
     */
    // $db->sucesso($modulo);
    
    
    //elabrev.localizadorgasto_acao
    
    // aqui fa�o a checagem da tabela localizadorgasto_acao
    // para cada a��o, verifico as regioes que ela atende. descubro o acaid em ppaacao_proposta e o c�digo da regiao e gravo na tabela localizadorgasto_acao
     $sql="select a.acaid, a.acacod,a.prsano from elabrev.ppaacao_proposta a where a.prsano='".$_REQUEST['anoorigem']."'";
	 $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas >= 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em vari�veis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
     		// achos as regioes que s�o atendidas no esquema monitora   		
     		$sql = "select regcod from monitora.acao where acasnrap='f' and acacod='$acacod' and prgano='$prsano'"; 
     		$RS2 = $db->record_set($sql);
	        $nlinhas2 = $db->conta_linhas($RS2);
	        if ($nlinhas2 >= 0)
	        {
	        	for ($ii=0; $ii<=$nlinhas2;$ii++)
   		       {
   		       	  $res = $db->carrega_registro($RS2,$ii);
     		      // a linha abaixo transforma em vari�veis todos os campos do array
     		      if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

     		    //  print 'unicod='.$unicod.'-----uniid='.$uniid.'<br>';
     		      $sql = "select acaid from elabrev.localizadorgasto_acao where acaid=$acaid and regcod='$regcod'";            
     		      $dados_reg= $db->recuperar($sql);
     		      if (! $dados_reg['acaid'])
     		      {
     		      	$sql = "insert into elabrev.localizadorgasto_acao (regcod,acaid) values ('$regcod',$acaid)";
     		      	$saida=$db->executar($sql);	
     		      }  
   		       }
	        }	
   		}
	 } 
     
     $db->commit();    
     
}


?>
